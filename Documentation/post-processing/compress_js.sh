#!/bin/bash

for f in ../../Release_Binary/documentation/html/*.js ../../Release_Binary/documentation/html/**/*.js; do
  echo "$f"
  java -jar yuicompressor-2.4.8.jar --nomunge -o "$f" "$f" 
done
