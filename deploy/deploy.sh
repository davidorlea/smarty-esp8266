#!/bin/bash

pio ci examples/SonoffS20/SonoffS20.cpp --lib="." --project-conf="deploy/KitchenSideboardLight.ini"
pio ci examples/Custom/Custom.cpp --lib="." --project-conf="deploy/OutdoorFrontdoorLight.ini"
