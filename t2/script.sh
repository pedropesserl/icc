#!/bin/bash

set -e

chmod u+x logs_likwid.sh
./logs_likwid.sh
chmod u+x formata_dados.sh
./formata_dados.sh
chmod u+x graficos.sh
./graficos.sh
