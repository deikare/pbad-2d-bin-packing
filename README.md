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
