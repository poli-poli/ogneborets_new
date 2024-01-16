#!/bin/bash

# e2e-test-a.sh
echo "Test e2e - try start authentic task A"
counter=0

while true; do
    counter=$((counter + 1))
    response=$(wget -qO- "http://0.0.0.0:8081/api" 2>/dev/null)
    
    if [ "$response" ]; then
        echo "FPS is accessible"
        break
    fi

    if [ "$counter" -gt 60 ]; then
        echo "Test failed (FPS not accessible, waiting timeout in 60 secs is reached)"
        exit 1
    fi

    echo "Waiting..."
    sleep 1
done

wget -qO- "http://0.0.0.0:8081/api?command=start&task=A" 2>/dev/null
echo "Start Task A"
sleep 2

counter=0
while true; do
    counter=$((counter + 1))
    logResponse=$(wget -qO- "http://0.0.0.0:8081/api?tasks_log=true" 2>/dev/null)
    
    if echo "$logResponse" | grep -q "Task A: started"; then
        echo "Test completed successfully"
        break
    fi

    if [ "$counter" -gt 60 ]; then
        echo "Test failed (waiting timeout in 60 secs is reached)"
        exit 1
    fi

    echo "Waiting..."
    sleep 1
done

# e2e-test-b.sh
echo "Test e2e - try to start not_authentic task B"
counter=0

while true; do
    counter=$((counter + 1))
    response=$(wget -qO- "http://0.0.0.0:8081/api" 2>/dev/null)
    
    if [ "$response" ]; then
        echo "FPS is accessible"
        break
    fi

    if [ "$counter" -gt 60 ]; then
        echo "Test failed (FPS not accessible, waiting timeout in 60 secs is reached)"
        exit 1
    fi

    echo "Waiting..."
    sleep 1
done

wget -qO- "http://0.0.0.0:8081/api?command=start&task=B" 2>/dev/null
echo "Start Task B"
sleep 2

counter=0
while true; do
    counter=$((counter + 1))
    logResponse=$(wget -qO- "http://0.0.0.0:8081/api?tasks_log=true" 2>/dev/null)
    
    if echo "$logResponse" | grep -q "Task B: not authentic"; then
        echo "Test completed successfully"
        break
    fi

    if [ "$counter" -gt 60 ]; then
        echo "Test failed (waiting timeout in 60 secs is reached)"
        exit 1
    fi

    echo "Waiting..."
    sleep 1
done