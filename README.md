## Snake WebAssembly igrica
C++ Snake igrica napisana korištenjem SDL2, SDL2_image te SDL2_ttf biblioteke.
Uz pomoć Emscripten kompajlera, kod igrice kompajlira se u WebAssembly te se tako
igrica može igrati na web-preglednicima.
Kako bi igricu pokrenuli lokalno, potrebno je prvotno kompajlirati igricu pokretanjem
Makefile-a naredbom "mingw32-make" ili pokretanjem naredbe koja se nalazi u Makefile-u 
(za obe naredbe potrebno je imati instaliran mingw32 na računalu):
    g++ -Isrc/Include -Lsrc/lib -o snake *.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image.
Za pokretanje igrice na web-pregledniku, potrebno je samo pokrenuti snake.html
datoteku na lokalnom serveru (npr. pokretanjem python naredbe "python -m http.server").
Uzivajte u igrici!
