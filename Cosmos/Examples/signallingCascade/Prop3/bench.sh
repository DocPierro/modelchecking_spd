#!/bin/bash

Cosmos MAPK_P3Reduit4.grml prop3RE.lha --const "L=3" --prism --prism-path /Applications/prism-4.0.2-osx64/bin/prism
Cosmos MAPK.grml prop3.lha --const "L=6,N=2" -r --max-run 1000000 --width 0 --njob 6
