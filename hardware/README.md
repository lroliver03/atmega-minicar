# atMEGA Minicar: Hardware

The hardware front of the project has to work on a few different things:
- **Component and module selection**: the components and modules used should be documented in the [documentation folder](./documentation).
- **Simulations**: Tinkercad will be used for firmware simulations and initial planning of circuit wiring.
- **Circuit schematics**: pin connections and wiring should be planned and drawn prior to circuit assembly and stored in the [circuit schematics folder](./schematics/wiring). If we choose to make a PCB, Kicad will be used for the board schematics in the [PCB schematics folder](./schematics/PCB).
- **Circuit casing**: the board, modules and other components will have a 3D-printed protective casing, stored in the [casing folder](./schematics/casing) within schematics.

The following components and modules will be used:
- [HC-50 Bluetooth to Serial module](../../main/hardware/documentation/HC-05%20Datasheet.pdf)
- [HC-SR04 Ultrasonic Distance Sensor module](../../main/hardware/documentation/HC-SR04-Ultrasonic%20User%20Guide.pdf) (w/ warning LED depending on obstacle proximity)
- H bridge for wheels (TBA)
- 360 deg Servo motors for wheels (TBA)
