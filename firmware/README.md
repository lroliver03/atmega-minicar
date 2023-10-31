# atMEGA Minicar: Firmware

The minicar's firmware will have the following features:
- **Speed control**: the speed of the car should be controlled remotely.
- **Distance reading**: the car should stop moving automatically if there is an obstacle a certain distance in front of it.
- **Bluetooth communication**: the minicar will be controlled via UART with the [HC-50 Bluetooth to Serial module](../main/hardware/documentation/HC-05%20Datasheet.pdf). Configurations and conventions are yet to be specified. The [software front](../software) is responsible for establishing communication with a controller device.