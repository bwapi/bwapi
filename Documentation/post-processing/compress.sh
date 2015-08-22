#!/bin/bash

pushd .
cd "${0%[\\/]*}"

sh compress_html.sh
sh compress_js.sh
sh compress_css.sh
sh compress_png.sh

popd