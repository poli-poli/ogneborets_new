# e2e-test-a.ps1
echo "Test e2e - try start authentic task A"
$counter = 0
while ($true) {
    $counter++
    $response = Invoke-WebRequest -Uri "http://localhost:8081/api" -UseBasicParsing -ErrorAction SilentlyContinue
    if ($response) {
        echo "FPS is accessible"
        break
    }
    if ($counter -gt 60) {
        echo "Test failed (FPS not accessible, waiting timeout in 60 secs is reached)"
        exit 1
    }
    echo "Waiting..."
    Start-Sleep -Seconds 1
}

Invoke-WebRequest -Uri "http://localhost:8081/api?command=start&task=A" -UseBasicParsing
echo "Start Task A"
Start-Sleep -Seconds 2

$counter = 0
while ($true) {
    $counter++
    $logResponse = Invoke-WebRequest -Uri "http://localhost:8081/api?tasks_log=true" -UseBasicParsing
    if ($logResponse.Content -match "Task A: started") {
        echo "Test completed successfully"
        break
    }
    if ($counter -gt 60) {
        echo "Test failed (waiting timeout in 60 secs is reached)"
        exit 1
    }
    echo "Waiting..."
    Start-Sleep -Seconds 1
}

# e2e-test-b.ps1
echo "Test e2e - try to start not_authentic task B"
$counter = 0
while ($true) {
    $counter++
    $response = Invoke-WebRequest -Uri "http://localhost:8081/api" -UseBasicParsing -ErrorAction SilentlyContinue
    if ($response) {
        echo "FPS is accessible"
        break
    }
    if ($counter -gt 60) {
        echo "Test failed (FPS not accessible, waiting timeout in 60 secs is reached)"
        exit 1
    }
    echo "Waiting..."
    Start-Sleep -Seconds 1
}

Invoke-WebRequest -Uri "http://localhost:8081/api?command=start&task=B" -UseBasicParsing
echo "Start Task B"
Start-Sleep -Seconds 2

$counter = 0
while ($true) {
    $counter++
    $logResponse = Invoke-WebRequest -Uri "http://localhost:8081/api?tasks_log=true" -UseBasicParsing
    if ($logResponse.Content -match "Task B: not authentic") {
        echo "Test completed successfully"
        break
    }
    if ($counter -gt 60) {
        echo "Test failed (waiting timeout in 60 secs is reached)"
        exit 1
    }
    echo "Waiting..."
    Start-Sleep -Seconds 1
}
