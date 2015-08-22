#!/bin/bash

for f in ../../Release_Binary/documentation/html/*.css ../../Release_Binary/documentation/html/**/*.css; do
  echo "$f"
  java -jar yuicompressor-2.4.8.jar -o "$f" "$f" 
done
