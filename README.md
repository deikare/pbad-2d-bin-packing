# pbad-2d-bin-packing

# pseudokod black boxa
  wejście: macierz wag sieci neuronowej
  
  dopóki itemy niepuste && możemy wstawiać:
    ymax = 0
    dla każdego j z typów_itemów:
        dla każdego i z cp:
            dla każdego k z {pion, poziom}:
                oblicz lewygórnyCP, prawydolnyCP, cechy
                oblicz y = neural_network(cechy)

                jeśli y > ymax:
                    zaktualizuj lgCP, pdCP, y, j

    usuń j z itemów
    wstaw lgCP, pdCP i usuń stare CP
    zaktualizuj średnią ocenę

itemki - lista, bo złożoność usuwania = O(1)
cp - lista, bo złożoność emplace przed pozycją = O(1)

## Cechy
obowiązkowe
* procent zmarnowanej powierzchni
* procent całkowitej powierzchni zajętej

opcjonalne:
* procent powierzchni wolnego prawego górnego prostokąta
* ile corner pointów
* ile pozostało poszczególnych pudełek

## Sieć neuronowa

Wagi to 3-wymiarowy wektor, gdzie waga w(i,j,k): 
- znajduje sie w i-tej warstwie
- wychodzi z j-tego neuronu
- wchodzi do k-tego neuronu

Do każdej warstwy prócz pierwszej dodany jest bias. Jest to wektor wag dodany do każdej kolejnej warsty.

Przykład sieci dla warstw o liczbie neuronów: 2/3/1:

{
  {{0.1, 0.2, 0.1},
   {0.2, 0.3, 0.1},
   {0.2, 0.3, 0.4}, // bias
  },
  {{0.1},
   {0.8},
   {0.2},
   {0.3}} // bias
};

## Integracja z libcmaes
* wykonać instrukcje z [wiki](https://github.com/CMA-ES/libcmaes/wiki) - opcjonalne, jeśli pliki z inc oraz lib nie są jeszcze dodane 
* sprawdzić, gdzie znajduje się paczka `eigen3` poleceniem `dpkg -S libeigen3-dev`
