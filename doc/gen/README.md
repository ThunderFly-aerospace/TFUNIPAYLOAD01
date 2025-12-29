# PCB

Board size: 70.61x50.29 mm (2.78x1.98 inches)

- This is the size of the rectangle that contains the board
- Thickness: 1.66 mm (65 mils)
- Material: FR4
- Finish: HAL lead-free
- Layers: 2
- Copper thickness: 35 µm

Solder mask: TOP / BOTTOM

- Color: Green

Silk screen: TOP / BOTTOM

- Color: White


Stackup:

| Name                 | Type                 | Color            | Thickness [µm]| Material        | Er        | Loss tan     |
|----------------------|----------------------|------------------|---------------|-----------------|-----------|--------------|
| F.SilkS              | Top Silk Screen      |                  |               |                 |           |              |
| F.Paste              | Top Solder Paste     |                  |               |                 |           |              |
| F.Mask               | Top Solder Mask      |                  |            10 |                 |           |              |
| F.Cu                 | copper               |                  |            35 |                 |           |              |
| dielectric 1         | core                 |                  |          1570 | FR4             |       4.5 |        0.020 |
| B.Cu                 | copper               |                  |            35 |                 |           |              |
| B.Mask               | Bottom Solder Mask   |                  |            10 |                 |           |              |
| B.Paste              | Bottom Solder Paste  |                  |               |                 |           |              |
| B.SilkS              | Bottom Silk Screen   |                  |               |                 |           |              |

# Important sizes

Clearance: 0.2 mm (8 mils)

Track width: 0.3 mm (12 mils)

- By design rules: 0.2 mm (8 mils)

Drill: 0.5 mm (20 mils)

- Vias: 0.5 mm (20 mils) [Design: 0.4 mm (16 mils)]
- Pads: 1.0 mm (39 mils)
- The above values are real drill sizes, they add 0.1 mm (4 mils) to plated holes (PTH)

Via: 0.8/0.4 mm (31/16 mils)

- By design rules: 0.8/0.3 mm (31/12 mils)
- Micro via: yes [0.2/0.1 mm (8/4 mils)]
- Buried/blind via: yes
- Total: 66 (thru: 66 buried/blind: 0 micro: 0)

Outer Annular Ring: 0.15 mm (6 mils)

- By design rules: 0.26 mm (10 mils)

Eurocircuits class: 4B
- Using min drill 0.5 mm for an OAR of 0.15 mm


# General stats

Components count: (SMD/THT)

- Top: 1/6 (SMD + THT)
- Bottom: 69/0 (SMD)

Defined tracks:

- 0.25 mm (10 mils)
- 0.3 mm (12 mils)
- 0.4 mm (16 mils)
- 0.5 mm (20 mils)
- 0.6 mm (24 mils)
- 0.7 mm (28 mils)
- 0.8 mm (31 mils)

Used tracks:

- 0.3 mm (12 mils) (37) defined: yes
- 0.4 mm (16 mils) (486) defined: yes
- 0.5 mm (20 mils) (22) defined: yes
- 0.7 mm (28 mils) (105) defined: yes

Defined vias:


Used vias:

- 0.8/0.4 mm (31/16 mils) (Count: 66, Aspect: 2.1 A) defined: no

Holes (excluding vias):

- 0.89 mm (35 mils) (30)
- 3.0 mm (118 mils) (4)

Oval holes:


Drill tools (including vias and computing adjusts and rounding):

- 0.5 mm (20 mils) (66)
- 1.0 mm (39 mils) (30)
- 3.1 mm (122 mils) (4)

Solder paste stats:

Using a paste with 87.75 % alloy, that has an specific gravity for the alloy of 7.4 g/cm³
and 1.0 g/cm³ for the flux. This paste has an specific gravity of  4.15 g/cm³.

The stencil thickness is  0.12 mm.

| Side   | Pads with paste | Area [mm²] | Paste [g] |
|--------|-----------------|------------|-----------|
| Total  |             219 |     253.53 |      1.26 |

Note: this is just an approximation to the theoretical value. Margins of the solder mask and waste aren't computed.



