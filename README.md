# TIPE-2023-Crowd-Simulations
Project files of my TIPE (Personal Initiative Project) made as I was studying in CPGE (Two-year classes to prepare for the competitive exams to the french "Grandes Ecoles") from 2021 to 2023. \
The project is assessed during the exams and received the grade of 13,8/20. \
See (french) :
- [MCOT](https://github.com/A-delta/TIPE-2023-Simulation-de-Foules/blob/main/presentation_TIPE/Mcot_louis.pdf)
- [Presentation](https://github.com/A-delta/TIPE-2023-Simulation-de-Foules/blob/main/presentation_TIPE/fichier_principal.pdf)
## Usage
### Compile
`gcc -o crowdsim -lm ./*.c`
### Run
- `runmp4 maps/your_map N`
- `rungif maps/your_map N`

where `N` is the number of initial agents.

## Example
`./runmp4 maps/simple_porte_test_obstacle.txt 100`

Each digit represents the number of agents on the square, a red segment points toward their desired exit.

https://github.com/A-delta/TIPE-2023-Simulation-de-Foules/assets/55986107/b6d90139-a3bf-4aae-b1cc-28a77aa769b3

![image](https://github.com/A-delta/TIPE-2023-Simulation-de-Foules/assets/55986107/57ea9ae2-8176-4500-8d9f-eb67ee4b8141)

## Settings
Generating large ppm files for each step takes a long time, you can change the scale of the output video relative to the map size by editing the `SCALE` macro in `image.c`, useful to speed up simulation on large maps or to get fancier results.
### SCALE = 10
![output](https://github.com/A-delta/TIPE-2023-Simulation-de-Foules/assets/55986107/1ae94229-d3ef-480e-af65-3516627cdafe)

### SCALE = 20
![output](https://github.com/A-delta/TIPE-2023-Simulation-de-Foules/assets/55986107/bd3a9efb-239a-4000-a53c-1153782bec8d)

### SCALE = 40
![output](https://github.com/A-delta/TIPE-2023-Simulation-de-Foules/assets/55986107/7ec7f2ee-8084-461c-afae-ae0784798097)
