$uri = "http://localhost:5000/tanque/logs"

$date = Get-Date
$body = @{
    logs = "teste1`nteste2`n"
    data = '{"id": "", "distance": ""}'
    datetime = $date.ToString("yyyy-MM-dd HH:mm:ss")
}

Invoke-RestMethod -Method Post -Uri $uri -Body $body