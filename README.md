# Multithreaded-Client-Server-App

#### Ky eshte nje aplikacion i thjeshte per komunikim mes klientave me nje server.
#### Aplikacioni perdor threads per menaxhimin e klientave (Klientave te ri, requests nga klientat dhe diskonektimin e tyre).
#### Aplikacioni eshte i programuar ne gjuhen C duke perdorur sistemin operativ Linux.


## Kompajllimi dhe Setup i projektit

### Per ta bere setup projektin duhet te ndjeken keto hapa
#### Krijoni dy fajlla: server.c dhe client.c
#### Fajllat duhet te krijohen ne te njejten directory
### Per te kompajlluar aplikacionin duhet te ndjeken keto hapa
#### Hapja e dy a po me shume terminaleve
#### Per kompajllimin e serverit duhet te shtypet komanda "gcc server.c -o server -pthread"
#### Per startimin e serverit duhet te shtype komanda ./server
#### Per kompajllimin e klientit duhet te shtypet komanda "gcc client.c -o client"
#### Per startimin e klientit duhet te shtypet komanda ./client


## Si punon aplikacioni?
### Per t'u konektuar me serverin, klienti duhet te shtyp mesazhin "Connect"
### Pas konektimit me serveri klienti mund te dergoje ne server qfaredo lloj mesazhi
### Per t'u diskonektuar me serverin, klienti duhet te shtyp mesazhin "Disconnect"


## Autoret
#### Astrit Krasniqi
#### Blert Osmani
#### Blerta Azemi
#### Diart Maraj
