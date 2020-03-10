Aby odpalić tego graficznego pingwina trzeba pobrać bibliotekę ncurses.

Otwieramy terminal na linuksie i wpisujemy: 
sudo apt-get install libncurses5-dev libncursesw5-dev

Nastepnie klasycznie otwieramy terminal w katalogu w którym mamy ten projekt, wpisujemy
make i następnie make run.

W tym repo są nieaktualne ai_move.h i ai_move.c ale to możecie sobie podmienić.
Nie polecam podmieniac maina bo wtedy nie bedzie widać tego trybu graficznego
(chyba że użyjecie funkcji w mainie z graphic_penguin.h).

Jak to działa:

Najpierw wyświetli się plansza w trybie graficznym. Strzałkami wybieramy pingwina do poruszenia.
Będzie się zmieniał jego kolorek. Nastepnie enterem zatwierdzamy pingwinka i możemy go ruszać.
Jak damy enter to nasz pingwin się przesunie, zbierze punkty i zmieniona plansza zostanie zapisana do
pliku.

