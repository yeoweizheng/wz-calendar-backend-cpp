#!/bin/bash
docker run --name wzcalendar -v $PWD/:/wzcalendar --network host -t -d wzcalendar:latest