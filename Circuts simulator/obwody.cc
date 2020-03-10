#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iterator>
#include <set>
#include <sstream>
#include <regex>

/** Ogólny zarys
    W rozwiązaniu do przetwarzania danych użylismy 4 typów kontenerów, są to:
    wektor, mapa, para oraz zbiór.
--------------------------------------------------------------------------------

  Mapa - w naszym rozwiązaniu korzystamy łacznie z 10 map. 5 z nich jest typu
  <string, set<int> > a 5 typu <set<int>, string>. Kluczem w pierwszej 5 jest
  typ elementu a wartoscią zbiór elementów tego typu o numerach zawartych w tym zbiórze
  (drugie 5 map jest odwróceniem pierwszych 5). Mapy trzymane są w tablicy map
  circuit components[5]. Pod odpowiednimi indeksami znajdują się:
  0 - trazystory;
  1 - diody;
  2 - rezystory;
  3 - kondensatory;
  4 - źródła napięcia.
--------------------------------------------------------------------------------

  Wektor - Wektor jest używany w momencie dzielenia wczytanej linii na wyrazy.
  Wektor (przy założeniu, że wczytana linia byla poprawna) ma zatem dlugość 4 lub 5.
  Pod odpowiednimi indeksami znajdują się:
  0 - rodzaj i numer elementu;
  1 - typ elementu;
  2,3,(4) - węzły do których podłączony jest element.
--------------------------------------------------------------------------------

  Zbiór - Zbior jest wykorzystywany do przechowywania: podlaczonych węzłów do co najmniej
  dwóch różnych elementów, węzłów podłączonych tylko do 1 elementu, numerów elementów
  odpowiednich typów.

--------------------------------------------------------------------------------

  Para - Para jest wykorzystywana do sprawnej transpozycji pierwszych 5 map w
  kolejne 5.

--------------------------------------------------------------------------------

*/

using namespace std;

namespace {

using regex_pattern = const string;
using component_id_set = set<string>;
using component_num_set = set<int>;
using component = vector<string>;
using circuit = map<string, set<int>>;
using transposed_circuit = map<set<int>, string>;
using component_nums_by_type = pair<string, set<int>>;
using type_by_component_nums = pair<set<int>, string>;
using node_set = set<int>;

regex_pattern number_pattern() {
  return "(0|[1-9][0-9]{0,8})";
}

regex_pattern component_type_pattern() {
  return "([A-Z]|[0-9])([A-Z]|[a-z]|[0-9]|,|\\/|-|)*";
}

regex valid_line() {
  regex_pattern transistor_line = string("\\s*") + "T" + number_pattern() + "\\s+"
  + component_type_pattern() + "(\\s+" + number_pattern() + "){3}" + "\\s*";
  regex_pattern non_transistor_line = string("\\s*") + "[CDER]" + number_pattern() + "\\s+"
  + component_type_pattern() + "(\\s+" + number_pattern() + "){2}" + "\\s*";
  regex valid_line(transistor_line + "|" + non_transistor_line);

  return valid_line;
}

void print_line_error_message(string *line, int line_num) {
  cerr << "Error in line " << line_num << ": " << *line << endl;
}

void read_lines_until_not_empty(string *line, int *line_num) {
  do {
    (*line_num)++;
  } while (getline(cin, *line) && (*line).empty());
}

/** Funkcja czyta z wejścia pierwszą niepustą linię, ignorując linie puste. Jeśli napotka eof lub
 * linia nie pasuje do wyrażenia regularnego odpowiadającego prawidłowej linii, kończy działanie.
 * W przeciwnym wypadku dodaje do wektora, reprezentującego element obwodu opisany w linii, kolejne
 * fragmenty opisu. Zwraca false, jeśli napotka eof.
 */
bool read_line(string *line, component *current, int *line_num) {
  read_lines_until_not_empty(line, line_num);

  if (cin.eof() && line->empty())
    return false;
  else if (!regex_match(*line, valid_line()))
    return true;

  istringstream line_stream(*line);

  copy(istream_iterator<string>(line_stream), istream_iterator<string>(), back_inserter(*current));

  return true;
}

/** Funkcja sprawdzająca, czy podany element jest tranzystorem.
 */
bool is_transistor(component *current) {
  if ((*current).empty() || (*current)[0].empty())
    return false;
  else
    return (*current)[0][0] == 'T';
}

/** Funcja sprawdzajaca, czy podany element ma odpowiedni rozmiar ze wzgledu na swoj typ
 * Dla tranzystora wynosi on 5, a dla pozostalych elemenów 4.
 */
bool has_correct_size(component *current) {
  const int transistor_size = 5;
  const int non_transistor_size = 4;

  return (is_transistor(current) && (*current).size() == transistor_size)
    || (*current).size() == non_transistor_size;
}

/** Funkcja sprawdzająca czy podany element jest podłączany tylko do jednego węzła.
 */
bool are_all_nodes_the_same(component *current) {
  const string node_1 = (*current)[2];
  const string node_2 = (*current)[3];

  if (is_transistor(current)) {
    const string node_3 = (*current)[4];

    return node_1 == node_2 && node_1 == node_3;
  } else {
    return node_1 == node_2;
  }
}

/** Funkcja sprawdza, czy podany element nie dubluje się.
 */
bool is_component_id_unique(component *current, component_id_set *added_components) {
  return (*added_components).find((*current)[0]) == (*added_components).end();
}

/** Funkcja sprawdza, czy podany wektor reprezentujący element obwodu, jest pusty.
 */
bool is_line_valid(component *current) {
  return !current->empty();
}

/** Funkcja sprawdza, czy wczytany element jest zgodny ze specyfikacjami zadania.
 */
bool is_line_valid(component *current, component_id_set *added_components) {
  return has_correct_size(current) && is_component_id_unique(current, added_components)
  && !are_all_nodes_the_same(current);
}

/** Funkcja dodaje element do zbioru istniejacych elementów.
 */
void add_id_to_component_set(component *current, component_id_set *added_components) {
  (*added_components).insert((*current)[0]);
}

/** Funkcja dodająca nowo wczytany węzeł do zbioru istniejacych węzłów.
 *  Funkcja sprawdza, czy nowo wczytany węzeł znajduje się w zbiorze
 *  połączonych węzłów. Jesli tak, to nic się nie dzieje. W p. p.
 *  sprawdza, czy węzeł ten istnieje, ale jest polaczony tylko z jednym elementem.
 *  Jesli nie istnieje, to jest dodawany do zbioru niepołączanych węzłów, a jeśli
 *  istnieje, to jest usuwany ze zbioru unconnected i dodawany do zbioru connected.
 */
void add_node_to_circuit_nodes(int node, node_set *connected, node_set *unconnected) {
  auto it = connected->find(node);

  if (it == connected->end()) {
    it = unconnected->find(node);

    if (it == unconnected->end()) {
      unconnected->insert(node);
    }
    else {
      unconnected->erase(it);
      connected->insert(node);
    }
  }
}

/** Funkcja dodająca węzły poprawnego elementu do zbiorów istniejacych elementów.
 *  Funkcja zamienia napis reprezentujacy węzły z wektora current na odpowiednie
 *  węzły. Nastepnie wywołuje funkcje dodawania do zbiorów tylko dla tych, które się
 *  różniły (zakladamy tutaj, że aby węzeł był poprawnie połączony, musi być połączony
 *  z co najmniej dwoma różnymi elementami).
 */
void add_component_nodes_to_circuit_nodes(component *current, node_set *connected,
                                          node_set *unconnected) {
  int node_1 = stoi((*current)[2]), node_2 = stoi((*current)[3]);

  add_node_to_circuit_nodes(node_1, connected, unconnected);

  if (node_2 != node_1)
    add_node_to_circuit_nodes(node_2, connected, unconnected);

  if (is_transistor(current)) {
    int node_3 = stoi((*current)[4]);

    if (node_3 != node_2 && node_3 != node_1)
      add_node_to_circuit_nodes(node_3, connected, unconnected);
  }
}

/** Funkcja dodająca numer identyfikatora i typ elementu do zbiorów numerów identyfikatorów
 * i typów.
 */
void add_component_id_and_type(component *current, circuit *components) {
  auto it = components->find((*current)[1]);
  int component_num = stoi((*current)[0].substr(1));

  if (it != components->end()) {
    it->second.insert(component_num);
  }
  else {
    component_num_set new_set;

    new_set.insert(component_num);
    components->insert(component_nums_by_type((*current)[1], new_set));
  }
}

/** Funkcja drukująca informacje o niepołączonych węzłach.
 */
void print_unconnected_node_warning(node_set *connected, node_set *unconnected) {
  if (connected->find(0) == connected->end() && unconnected->find(0) == unconnected->end())
    unconnected->insert(0);

  if (!unconnected->empty()) {
    auto it = unconnected->begin();

    cerr << "Warning, unconnected node(s): " << *it++;

    while (it != unconnected->end())
      cerr << ", " << *it++;

    cerr << endl;
  }
}

/** Funkcja przypisująca elementowi odpowiedni "priorytet" wypisywania tzn. jeśli
 * priorytet jest mniejszy to elementy tego typu powinny byc wypisane wcześniej.
 */
int component_symbol_num(char symbol) {
  switch (symbol) {
  case 'T':
    return 0;
  case 'D':
    return 1;
  case 'R':
    return 2;
  case 'C':
    return 3;
  case 'E':
    return 4;
  default:
    return -1;
  }
}

char component_num_symbol(int num) {
  switch (num) {
  case 0:
    return 'T';
  case 1:
    return 'D';
  case 2:
    return 'R';
  case 3:
    return 'C';
  case 4:
    return 'E';
  default:
    return '\0';
  }
}

/** Funkcja rozdzielajaca podfunkcjom obowiązki dodania informacji o elemencie do odpowiednich
 * zbiorów informacji.
 */
void add_component_to_circuit(component *current, component_id_set* added_components,
                              node_set *connected, node_set *unconnected,
                              circuit *circuit[]) {
  add_id_to_component_set(current, added_components);
  add_component_nodes_to_circuit_nodes(current, connected, unconnected);
  add_component_id_and_type(current, circuit[component_symbol_num((*current)[0][0])]);
}

/** Funkcja przygotowuje zgromadzone dane do wypisania.
 *  Funkcja dokonuje zamiany na mapie klucza z wartoscia.
 *  Przy wypisywaniu kluczem będzie zbiór numerów konkretnych rodzajów elementów.
 */
void prepare_components_to_print(circuit *components[], transposed_circuit *ready_to_print[]) {
  for (int i = 0; i < 5; i++) {
    for (auto it = (components[i])->begin(); it != components[i]->end(); it++)
      ready_to_print[i]->insert(type_by_component_nums(it->second, it->first));
  }
}


/** Wypisywanie w liniach kolejnych elementów należących do obwodu.
 */
void print_circuit_components(transposed_circuit *ready_to_print[]) {
  for (int i = 0; i < 5; i++) {
    for (auto it_1 = (ready_to_print[i])->begin(); it_1 != ready_to_print[i]->end(); it_1++) {
      auto it_2 = it_1->first.begin();

      cout << component_num_symbol(i) << *it_2++;

      while (it_2 != it_1->first.end())
        cout << ", " << component_num_symbol(i) << *it_2++;

      cout << ": " << it_1->second << endl;
    }
  }
}

/** Funkcje read_data czyta w pętli kolejne linie z wejścia.
 * Dla każdej odczytanej linii sprawdzane są warunki jej poprawności.
 * Jeżeli wczytana linia zawiera poprawne dane następuje dodanie wczytanego elementu
 * do zbiorów poprawnych elementów, w p.p wypisywana jest informacja o błędzie.
 */
void read_data(component_id_set *added_components, node_set *connected, node_set *unconnected,
               circuit *circuit[]) {
  string line;
  int line_num = 0;
  component current;

  while (read_line(&line, &current, &line_num)) {
    if (is_line_valid(&current) && is_line_valid(&current, added_components))
      add_component_to_circuit(&current, added_components, connected, unconnected, circuit);
    else
      print_line_error_message(&line, line_num);

    current.clear();
  }
}

}

int main() {
  component_id_set added_components;
  node_set connected, unconnected;
  circuit components[5];
  circuit *component_ptrs[5];
  transposed_circuit ready_to_print[5];
  transposed_circuit *ready_to_print_ptrs[5];

  for (int i = 0; i < 5; i++) {
    component_ptrs[i] = &(components[i]);
    ready_to_print_ptrs[i] = &(ready_to_print[i]);
  }

  read_data(&added_components, &connected, &unconnected, component_ptrs);

  prepare_components_to_print(component_ptrs, ready_to_print_ptrs);
  print_circuit_components(ready_to_print_ptrs);
  print_unconnected_node_warning(&connected, &unconnected);
}
