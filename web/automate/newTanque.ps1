$uri = "http://localhost:5000/tanque/1"

$date = Get-Date
$body = @{
    distance = 20
    datetime = $date.ToString("yyyy-MM-dd HH:mm:ss")
}

Invoke-RestMethod -Method Post -Uri $uri -Body $body