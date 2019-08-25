#!/bin/bash

pio run --environment custom-v3-current --target upload --upload-port 192.168.178.40       # OutdoorFrontdoorLight
pio run --environment custom-v3-current --target upload --upload-port 192.168.178.45       # OutdoorBackdoorLight
pio run --environment custom-v2-current --target upload --upload-port 192.168.178.35       # KitchenCeilingLight
pio run --environment custom-v2-current --target upload --upload-port 192.168.178.32       # LivingRoomCeilingLight
pio run --environment custom-v2-current --target upload --upload-port 192.168.178.38       # BedroomCeilingLight
pio run --environment custom-v2-current --target upload --upload-port 192.168.178.36       # HallCeilingLight
pio run --environment sonoff-s20-current --target upload --upload-port 192.168.178.33      # LivingRoomSofaLight
pio run --environment sonoff-s20-current --target upload --upload-port 192.168.178.28      # KitchenCabinetLight
pio run --environment sonoff-basic-current --target upload --upload-port 192.168.178.47    # KitchenSideboardLight
pio run --environment sonoff-basic-current --target upload --upload-port 192.168.178.44    # BedroomAmbientLight
pio run --environment sonoff-basic-current --target upload --upload-port 192.168.178.46    # BedroomBedLight
