#!/bin/bash
MY_DIRNAME=$(dirname $0)
cd $MY_DIRNAME

java -jar CodeGenerator.jar CodeGenerator "schemas/common_response.adkschema"
