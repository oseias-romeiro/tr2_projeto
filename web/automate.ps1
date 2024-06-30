# Exemplo de post para passar os dados de um tanque de combustivel
Invoke-RestMethod -Method Post -Uri "http://localhost:5000/tanque/2" -Body @{
    distance = 20
}
