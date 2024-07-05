#!/bin/bash
uri="http://localhost:5000/tanque/1"
date=$(date +"%Y-%m-%d %H:%M:%S")
body="distance=20&datetime=$date"

echo $(curl -d "$body" -X POST "$uri")