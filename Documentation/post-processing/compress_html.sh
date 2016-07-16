#!/bin/bash

for f in ../../Release_Binary/documentation/html/*.html ../../Release_Binary/documentation/html/**/*.html; do
  echo "$f"
  java -jar htmlcompressor-1.5.3.jar \
    --remove-intertag-spaces \
    --simple-doctype \
    --remove-style-attr \
    --remove-link-attr \
    --remove-script-attr \
    --remove-form-attr \
    --remove-input-attr \
    --simple-bool-attr \
    --remove-http-protocol \
    --remove-https-protocol \
    --compress-js \
    --compress-css \
    -o "$f" "$f" 
done

