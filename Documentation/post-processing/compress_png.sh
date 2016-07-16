#!/bin/bash

for f in ../../Release_Binary/documentation/html/*.png ../../Release_Binary/documentation/html/**/*.png; do
  echo "$f"
  pngcrush -reduce -brute -q "$f" "$f.crushed"
  mv "$f.crushed" "$f"
done
