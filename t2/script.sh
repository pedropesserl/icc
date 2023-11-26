#!/bin/bash

set -e

chmod u+x scripts/logs_likwid.sh
scripts/logs_likwid.sh
chmod u+x scripts/formata_dados.sh
scripts/formata_dados.sh
chmod u+x scripts/graficos.sh
scripts/graficos.sh
