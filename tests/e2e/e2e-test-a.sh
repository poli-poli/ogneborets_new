# e2e-test-a.sh
echo "E2E Test A - Start authentic task A"
counter=0
while true; do
    ((++counter))
    curl -s "http://localhost:8081/api"
    exit_code=$?
    if [ ${exit_code} -eq 0 ]; then
        echo "Service is accessible"
        break
    fi
    if [ ${counter} -gt 60 ]; then
        echo "Test failed (Service not accessible, waiting timeout in 60 secs is reached)"
        exit 1
    fi
    echo "Waiting..."
    sleep 1
done

curl -s "http://localhost:8081/api?command=start&task=A" 1>/dev/null && echo "Started Task A"
exit_code=$?
if [ ${exit_code} -ne 0 ]; then
    echo "Unable to connect to service on localhost:8081"
    exit 1
fi

sleep 2
counter=0
while true; do
    ((++counter))
    curl -s "http://localhost:8081/api?tasks_log=true" | tail -1 | grep "Task A: started"
    exit_code=$?
    if [ ${exit_code} -eq 0 ]; then
        echo "Test A completed successfully"
        break
    fi
    if [ ${counter} -gt 60 ]; then
        echo "Test A failed (waiting timeout in 60 secs is reached)"
        exit 1
    fi
    echo "Waiting..."
    sleep 1
done
