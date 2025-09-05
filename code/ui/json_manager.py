## @package json_manager
#  Handles importing/exporting @ref Slot (s) from/to JSON files
#
#  @author Jose Manuel Garcia Cazorla <jmgarcaz@correo.ugr.es>


import json
from pwmbox import PWM, Slot


## Stores the @ref Slot in JSON format
class json_file:
    ## Constructor
    #  @param self Object pointer
    #  @param path Path of the file to be loaded/saved
    def __init__(self, path: str = ".") -> None:
        self.path = path
        self.data = {}

    ## Print format
    #  @param self Object pointer
    #  @return str To be printed
    def __str__(self) -> str:
        return str(self.data)

    ## Loads the file from the set path
    #  @param self Object pointer
    def load(self) -> None:
        with open(self.path) as file:
            self.data = json.load(file)

    ## Converts the data to a single @ref Slot
    #  Data must be loaded from a file/@ref Slot beforehand
    #  @param self Object pointer
    #  @return Slot Loaded slot
    def to_slot(self) -> Slot:
        pwms: list[PWM] = []

        for i in range(0, 8):
            pwms.append(
                PWM(
                    self.data["pwm" + str(i + 1)]["name"][:19],
                    self.data["pwm" + str(i + 1)]["mode"],
                    self.data["pwm" + str(i + 1)]["frq"],
                    self.data["pwm" + str(i + 1)]["dty"],
                    self.data["pwm" + str(i + 1)]["phs"],
                )
            )

        slot = Slot(self.data["name"][:11], pwms)

        return slot

    ## Converts the data to a list of @ref Slot
    #  Data must be loaded from a file/@ref Slot beforehand
    #  @param self Object pointer
    #  @return list[Slot] Loaded slots
    def to_slots(self) -> list[Slot]:
        num_slots = self.data["num_slots"]
        slots: list[Slot] = []

        for i in range(num_slots):
            pwms = []

            for j in range(8):
                pwms.append(
                    PWM(
                        self.data["slot " + str(i + 1)]
                        ["pwm " + str(j + 1)]["name"][:19],
                        self.data["slot " + str(i + 1)]
                                 ["pwm " + str(j + 1)]["mode"],
                        self.data["slot " + str(i + 1)]
                                 ["pwm " + str(j + 1)]["frq"],
                        self.data["slot " + str(i + 1)]
                                 ["pwm " + str(j + 1)]["dty"],
                        self.data["slot " + str(i + 1)]
                                 ["pwm " + str(j + 1)]["phs"],
                    )
                )

            slots.append(Slot(self.data["slot " + str(i + 1)]["name"][:11], pwms))

        return slots

    ## Gets the data from a single @ref Slot
    #  @param self Object pointer
    #  @param slot To be loaded
    def from_slot(self, slot: Slot) -> None:
        self.data = slot.to_json()

    ## Gets the data from a list of @ref Slot
    #  @param self Object pointer
    #  @param slots To be loaded
    def from_slots(self, slots: list[Slot]) -> None:
        self.data = dict(num_slots=len(slots))

        for i in range(len(slots)):
            self.data["slot " + str(i + 1)] = slots[i].to_json()

    ## Saves the file to the set path
    #  @param self Object pointer
    def save(self) -> None:
        with open(self.path, "w") as file:
            json.dump(self.data, file, indent=4)
