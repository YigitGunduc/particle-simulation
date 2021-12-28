# particle-simulation

Particle simulation which can simulate water and sand. Built with C and SDL

## demo 


sand simulation

https://user-images.githubusercontent.com/63921693/147489513-e34c50e8-f93f-4326-ab8f-9e5fe78497e0.mp4


water + sand simulation

https://user-images.githubusercontent.com/63921693/147489533-9cb11aa6-1634-49c5-be7d-89ab65b74832.mp4


## Installation
```bash
git clone https://github.com/YigitGunduc/particle-simulation.git

cd particle-simulation/
```

## Building

Building the sand simulation alone
```bash
bash build.sh
```

Building the water + sand simulation
```bash
bash build.sh --water
```

executable will be present in the ./build/
to change the name out the build the change the ```BUILD_DIR``` variable in the 
```build.sh```


## License
[MIT](https://choosealicense.com/licenses/mit/)
