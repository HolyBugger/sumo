#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
# Copyright (C) 2008-2018 German Aerospace Center (DLR) and others.
# This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v2.0
# which accompanies this distribution, and is available at
# http://www.eclipse.org/legal/epl-v20.html
# SPDX-License-Identifier: EPL-2.0

# @file    runner.py
# @author  Mirko Barthauer (Technische Universitaet Braunschweig)
# @date    2018-09-27
# @version $Id$
import os
import sys

SUMO_HOME = os.path.join(os.path.dirname(__file__), "..", "..", "..", "..", "..")
sys.path.append(os.path.join(os.environ.get("SUMO_HOME", SUMO_HOME), "tools"))

import traci
import sumolib

sumoBinary = sumolib.checkBinary('sumo')
traci.start([sumoBinary, 
    "-n", "input_net.net.xml", "-r",
    "input_routes.rou.xml",
    "--no-step-log",
    ])

vehHasLeft = False
vehHasArrived = False
vehID = 'v1'
route = []
step = 0
resultList = [("step", "currentEdge", "targetEdge", "distoToTargetEdge")]

while not vehHasLeft:
    if not vehHasArrived:
        arrivedVehicles = traci.simulation.getDepartedIDList()
        vehHasArrived = vehID in arrivedVehicles
        route = traci.vehicle.getRoute(vehID)
    
    if vehHasArrived:
        remainingDist = traci.vehicle.getDrivingDistance(vehID, route[-1], 0.0)
        currentEdge = traci.vehicle.getRoadID(vehID)
        vehHasLeft = currentEdge == route[-1]
        
        if not vehHasLeft:
            resultList.append((str(step), currentEdge, route[-1], str(remainingDist)))
    step += 1
    traci.simulationStep()
traci.close()

# print distance to target edge of the vehicle's route
for line in resultList:
    print("%s" % ";".join(line))
