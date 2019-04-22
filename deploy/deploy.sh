#!/bin/bash

pio ci examples/CustomV3/CustomV3.cpp --lib="." --project-conf="deploy/OutdoorFrontdoorLight.ini"
pio ci examples/CustomV3/CustomV3.cpp --lib="." --project-conf="deploy/OutdoorBackdoorLight.ini"
pio ci examples/CustomV2/CustomV2.cpp --lib="." --project-conf="deploy/KitchenCeilingLight.ini"
pio ci examples/CustomV2/CustomV2.cpp --lib="." --project-conf="deploy/LivingRoomCeilingLight.ini"
pio ci examples/CustomV2/CustomV2.cpp --lib="." --project-conf="deploy/HallCeilingLight.ini"
pio ci examples/SonoffS20/SonoffS20.cpp --lib="." --project-conf="deploy/LivingRoomSofaLight.ini"
pio ci examples/SonoffS20/SonoffS20.cpp --lib="." --project-conf="deploy/KitchenCabinetLight.ini"
pio ci examples/SonoffBasic/SonoffBasic.cpp --lib="." --project-conf="deploy/KitchenSideboardLight.ini"
pio ci examples/SonoffBasic/SonoffBasic.cpp --lib="." --project-conf="deploy/BedroomAmbientLight.ini"
pio ci examples/SonoffBasic/SonoffBasic.cpp --lib="." --project-conf="deploy/BedroomBedLight.ini"
