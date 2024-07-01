# Exemplo de post para passar os dados de um tanque de combustivel
Invoke-RestMethod -Method Post -Uri "http://tr2.alwaysdata.net/tanque/2" -Body @{
    distance = 25
}
