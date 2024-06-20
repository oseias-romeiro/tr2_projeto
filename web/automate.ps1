# Exemplo de post para passar os dados de um tanque de combustivel
Invoke-RestMethod -Method Post -Uri "http://localhost:5000/tanque/1" -Body @{
    capacidade = 100
    nivel = 100
}
