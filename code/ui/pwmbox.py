## @package pwmbox
#  Provides the necessary objects to interact with a PWM Box
#
#  @author Jose Manuel Garcia Cazorla <jmgarcaz@correo.ugr.es>

import re
import time

from serial import Serial
from serial.tools.list_ports import comports


## Defines the representation of a PWM
class PWM:
    ## Constructor
    #  @param self Object pointer
    #  @param name Name of the PWM
    #  @param mode 0 = OFF, 1 = PWM, 2 = ON
    #  @param frq Frequency of the signal (0 - 400 Hz)
    #  @param dty Duty cycle of the signal (0 - 100%)
    #  @param phs Phase of the signal (-50 - 50%)
    def __init__(
        self, name: str = "", mode: int = 0, frq: int = 0, dty: int = 0, phs: int = 0
    ) -> None:
        self.name = name
        self.mode = mode
        self.frq = frq
        self.dty = dty
        self.phs = phs

    ## Print format
    #  @param self Object pointer
    #  @return str To be printed
    def __str__(self) -> str:
        string =  "PWM: " + self.name + "\n"
        string += "----------" + "\n"
        string += "Mode: " + str(self.mode) + "\n"
        string += "Frq: " + str(self.frq) + "\n"
        string += "Dty: " + str(self.dty) + "\n"
        string += "Phs: " + str(self.phs) + "\n"

        return string

    ## "Less than" comparison method
    #  @param self Object pointer
    #  @param other Another object's pointer
    #  @return bool Whether self is less than other or not
    def __lt__(self, other) -> bool:
        return self.name.lower() < other.name.lower()

    ## JSON format
    #  Used by @ref json_manager when exporting to a file
    #  @param self Object pointer
    #  @return dict Containing a JSON representation of the PWM
    def to_json(self) -> dict:
        return dict(
            name=self.name, mode=self.mode, frq=self.frq,
            dty=self.dty, phs=self.phs
        )


## Defines the representation of a memory slot
class Slot:
    ## Constructor
    #  @param self Object pointer
    #  @param name Name of the slot
    #  @param pwms List of PWMs the slot will contain
    def __init__(self, name: str = "", pwms: list[PWM] = []) -> None:
        self.name = name
        self.pwms = pwms

    ## Print format
    #  @param self Object pointer
    #  @return str To be printed
    def __str__(self) -> str:
        string =  "Slot: " + self.name + "\n"
        string += "====================\n"

        for i in range(len(self.pwms)):
            string += self.pwms[i].__str__()

            if i != len(self.pwms) - 1:
                string += "\n"

        string += "====================\n"

        return string

    ## "Less than" comparison method
    #  @param self Object pointer
    #  @param other Another object's pointer
    #  @return bool Whether self is less than other or not
    def __lt__(self, other) -> bool:
        return self.name.lower() < other.name.lower()

    ## JSON format
    #  Used by @ref json_manager when exporting to a file
    #  @param self Object pointer
    #  @return dict Containing a JSON representation of the Slot
    def to_json(self) -> dict:
        pwms = dict(name=self.name)

        for i in range(len(self.pwms)):
            pwms["pwm " + str(i + 1)] = self.pwms[i].to_json()

        return pwms


## Defines the representation of the PWM Box, providing methods to interact with it
class PWMBox:
    ## Constructor
    #  @param self Object pointer
    def __init__(self) -> None:
        self.NUM_PWMS = 8

        self.serial: Serial = None

        self.port: str = None
        self.description: str = None
        self.serial_num: int = None
        self.hw_version: float = None
        self.sw_version: float = None
        self.default_slot: int = None
        self.password: list[int] = None
        self.max_slots: int = None

        self.slots: list[Slot] = []

        # Find the device
        self.connect()

        # If we find it, get its info
        if self.serial is not None:
            self.get_info()
            self.get_password()
            self.get_slots()

    ## Finds the device and connects to it via the serial port
    #  @param self Object pointer
    def connect(self) -> None:
        found = False

        for i in comports():
            # Use the device description to narrow our search
            if re.search("usb.serial", i.description, re.IGNORECASE):
                candidate = Serial(port=i.device, baudrate=115200, timeout=1)


                time.sleep(2)  # Give the device time to wake up

                candidate.write(("^?,@\n").encode())  # Initial handshake
                response = candidate.read_until().decode()

                if "^!,@" in response:
                    self.serial = candidate

                    self.serial.timeout = None
                    self.port = i.device
                    self.description = i.description

                    found = True
                else:
                    candidate.close()

            if found is True:
                break

    ## Gets the basic device information
    #  @param self Object pointer
    def get_info(self) -> None:
        if self.serial is None:
            return

        self.serial.write(("^?,i\n").encode())  # Info
        response = self.serial.read_until().decode()

        m = re.search(r"\^!,i,(.*),(.*),(.*),(.*),(.*)", response)

        self.serial_num = int(m.group(1))
        self.hw_version = float(m.group(2))
        self.sw_version = float(m.group(3))

        if m.group(4) != "n":  # If there's a default slot set
            self.default_slot = int(m.group(4))

        self.max_slots = int(m.group(5))

    ## Gets the device password
    #  @param self Object pointer
    def get_password(self) -> None:
        if self.serial is None:
            return
        
        self.serial.write("^?,c\n".encode())  # Password
        response = self.serial.read_until().decode()

        m = re.match(r"\^!,c,(.*),(.*),(.*)", response)

        if m.group(1) == "n":  # If the password isn't set
            self.password = [-1, 0, 0]
        else:
            self.password = [int(m.group(1)), int(m.group(2)), int(m.group(3))]

    ## Gets the device's slots
    #  @param self Object pointer
    def get_slots(self) -> None:
        if self.serial is None:
            return
        
        # If slots are already loaded, delete them
        if self.slots:
            self.slots.clear()

        self.serial.write("^?,s\n".encode())  # Slots
        response = self.serial.read_until().decode()

        num_slots = int(re.match(r"\^!,n,(.*)", response).group(1))

        for i in range(0, num_slots):
            response = self.serial.read_until().decode()

            m = re.match(r"\^!,s,(.*),(.*)", response)
            slot_idx = int(m.group(1))
            slot_name = str(m.group(2))

            if int(slot_idx) != i:
                print("Missing slot " + str(i))

            pwms: list[PWM] = []

            for j in range(0, self.NUM_PWMS):
                
                response = self.serial.read_until().decode()

                m = re.match(r"\^!,p,(.*),(.*),(.*),(.*),(.*),(.*)", response)
                pwm_idx = int(m.group(1))
                pwm_name = str(m.group(2))
                pwm_mode = int(m.group(3))
                pwm_frq = float(m.group(4)) / 10
                pwm_dty = int(m.group(5))
                pwm_phs = int(m.group(6))

                if pwm_idx != j:
                    print("Missing PWM " + str(j))

                pwms.append(PWM(pwm_name, pwm_mode, pwm_frq, pwm_dty, pwm_phs))

            self.slots.append(Slot(slot_name, pwms))

    ## Send the device a new password
    #  @param self Object pointer
    #  @param new Password to be sent
    def set_password(self, new: list[int]) -> None:
        for i in range(3):
            self.password[i] = new[i]

        self.serial.write((
            "^!,c," +
            str(new[0]) + "," +
            str(new[1]) + "," +
            str(new[2]) + "\n"
        ).encode())

    ## Send the device a new default slot
    #  @param self Object pointer
    #  @param new Default slot to be sent
    def set_default(self, new: int) -> None:
        self.default_slot = new
        self.serial.write(("^!,d," + str(self.default_slot) + "\n").encode())

    ## Send slots to the device
    #  @param self Object pointer
    def set_slots(self) -> None:
        self.serial.write(("^!,n," + str(len(self.slots)) + "\n").encode())

        time.sleep(0.5)  # Device needs time to process data

        for i in range(len(self.slots)):
            self.serial.write((
                "^!,s," +
                str(i) + "," +
                self.slots[i].name + "\n"
            ).encode())

            time.sleep(0.5)  # Device needs time to process data

            for j in range(self.NUM_PWMS):
                self.serial.write((
                    "^!,p," +
                    str(j) + "," +
                    self.slots[i].pwms[j].name + "," +
                    str(int(self.slots[i].pwms[j].mode)) + "," +
                    str(int(self.slots[i].pwms[j].frq * 10)) + "," +
                    str(int(self.slots[i].pwms[j].dty)) + "," +
                    str(int(self.slots[i].pwms[j].phs)) + "\n"
                ).encode())

                time.sleep(0.5)  # Device needs time to process data

