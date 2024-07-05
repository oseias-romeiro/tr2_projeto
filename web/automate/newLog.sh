#!/bin/bash

uri="http://localhost:5000/tanque/logs"
datetime=$(date +"%Y-%m-%d %H:%M:%S")
body="logs=teste1
teste
&data={\"id\": \"\", \"distance\": \"\"}&datetime=$datetime"

echo $(curl -d "$body" -X POST "$uri")
