#!/bin/bash

pio ci examples/CustomV1/CustomV1.cpp --lib="." --project-conf="deploy/OutdoorFrontdoorLight.ini"
pio ci examples/SonoffS20/SonoffS20.cpp --lib="." --project-conf="deploy/KitchenSideboardLight.ini"
pio ci examples/SonoffS20/SonoffS20.cpp --lib="." --project-conf="deploy/LivingRoomSofaLight.ini"
pio ci examples/SonoffS20/SonoffS20.cpp --lib="." --project-conf="deploy/BedroomAmbienceLight.ini"
