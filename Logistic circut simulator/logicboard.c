#include <stdio.h>
#include <stdlib.h>
#include "logicboard.h"

#define ERR   -1
#define ENTER  0
#define AND    1
#define OR     2
#define NOT    3
#define SHOW   4
#define TEST   5
#define END    6
#define TRUE   1
#define FALSE  0


struct _node_list_;
struct _node_;
struct _logic_board_;



//Struktura opisujaca pojedynczy wezel w ukladzie logicznym
typedef struct _node_ {
    unsigned int number;
    int type;
    int input_byte;
    struct _node_list_* fathers;
    struct _node_* left_son;
    struct _node_* right_son;
}NODE;

//Lista wezlow
typedef struct _node_list_ {
    NODE* node;
    struct _node_list_* next;
}NODE_LIST;

//Struktura reprezentujaca uklad logiczny
typedef struct _logic_board_ {
    NODE* trunk;
    NODE_LIST* all_nodes;
    NODE_LIST* enters;
    int result;
}LOGIC_BOARD;






//Globalna zmienna statyczna z danymi o ukladzie, pierwotnie NULL
static LOGIC_BOARD* logic_board = NULL;

/** \brief Funkcja tworzaca nowy element listy wezlow.
 *         Funkcja alokuje pamiec na nowy element typu NODE_LIST.
           W przypadku bledu alokacji pamieci uzywana jest funkcja
           exit z kodem powrotu 1.

 * \param node - wskaznik na wezel ktory ma byc w elemencie listy.
 * \return Wskaznik na nowo utworzony element.
 *
 */
static NODE_LIST* create_list_element(NODE* node) {

    NODE_LIST* new_element = malloc(sizeof(NODE_LIST));

    if(new_element == NULL) {
        fprintf(stderr, "FATAL: Blad alokacji pamieci\n");
        exit(1);
    }

    new_element->next = NULL;
    new_element->node = node;

    return new_element;
}



/** \brief Funkcja toworzaca nowy wezel z ukladu logicznego.
 *         Funkcja tworzy nowy wezel do ukladu logicznego.
           Wezel otrzymuje identyfikator oraz typ w momecie
           tworzenia w tej wlasnie funkcji. Jesli brakuje pamieci
           by utowrzyc nowy wezel, wywolywana jest funkcja systemowa
           exit z kodem wyjscia 1.
 * \param number - identyfikator wezla.
 * \param type - typ tworzonego wezla.
 * \return Wskaznik na nowo utworzony wezel.
 *
 */
static NODE* create_empty_node(unsigned int number, int type) {

    NODE* enter_node = malloc(sizeof(NODE));

    if(enter_node == NULL) {
        fprintf(stderr, "FATAL: Blad alokacji pamieci\n");
        exit(1);
    }

    enter_node->type = type;
    enter_node->input_byte = -1;
    enter_node->number = number;
    enter_node->left_son = NULL;
    enter_node->right_son = NULL;
    enter_node->fathers = NULL;

    return enter_node;
}


/** \brief Funkcja tworzaca nowy uklad logiczny.
           Funkcja tworzy nowy uklad logiczny. Jest ona wywolywana
           w momencie pierwszego dodania wezla do ukladu. Alokuje
           pamiec potrzebna do storzenia struktury i ustawia wszystkie
           wartosci wskaznikow na NULL.

 * \return Wskaznik na nowo utworzona strukture.
 *
 */
static LOGIC_BOARD* create_board() {

    LOGIC_BOARD* new_board = malloc(sizeof(LOGIC_BOARD));

    if(new_board == NULL) {
        fprintf(stderr, "FATAL: Blad alokacji pamieci\n");
        exit(1);
    }

    new_board->all_nodes = NULL;
    new_board->enters = NULL;
    new_board->trunk = NULL;

    return new_board;
}

/** \brief Funkcja wyswietlajaca pojedynczy wezel.
 *         Funkcja wyswietla wezel uzytownikowi wedlug podanego formatu
           opisanemu w tresci zadania. W zaleznosci od typu wezla funkcja
           decyduje o innym przedstawieniu wezla.

 * \param node - Wskaznik na wezel ktory ma byc wyswietlony uzytkownikowi.
 *
 */
static void display_node(NODE* node) {

    if(node->type == AND)
        printf("%u=%uand%u\n",node->number, node->left_son->number, node->right_son->number);
    else if(node->type == OR)
        printf("%u=%uor%u\n",node->number, node->left_son->number, node->right_son->number);
    else if(node->type == ENTER)
        printf(" %u", node->number);
    else if(node->type == NOT)
        printf("%u=not%u\n",node->number, node->left_son->number);

}




void show_board() {

    if(logic_board == NULL || logic_board->trunk == NULL) {
        printf("Uklad logiczny jest pusty\n");
        return;
    }

    NODE_LIST* beginning = logic_board->all_nodes;

    while(beginning != NULL) {
        display_node(beginning->node);
        beginning = beginning->next;
    }

    beginning = logic_board->enters;

    printf("Wejscia:");

    while(beginning != NULL) {
        display_node(beginning->node);
        beginning = beginning->next;
    }

    printf("\nWyjscie: %u\n", logic_board->trunk->number);

}


/** \brief Funkcja wsortowywujaca element do listy elementow.
 *         Funkcja wsortowywuje element do listy elementow.
           Lista uporzadkowana jest w sposob rosnacy. Funkcja dziala
           w sposob rekurencyjny. Nigdy nie dodaje elementu jako pierwszy
           na liscie.
 * \param to_add - wskaznik na element ktory ma byc dodany do listy.
 * \param list - wskaznik na liste do ktorej dodajemy element.
 *
 */
static void sort_into_list(NODE_LIST* list, NODE* to_add) {

    if(list->next == NULL) {
        NODE_LIST* new_element = create_list_element(to_add);
        list->next = new_element;
        return;
    }

    if(list->next->node->number > to_add->number) {
        NODE_LIST* new_element = create_list_element(to_add);
        new_element->next = list->next;
        list->next = new_element;
        return;
    }

    sort_into_list(list->next, to_add);
}



/** \brief Funkcja dodajaca element do listy.
 *         Funkcja uwzglednia przypadki gdy dodwany element ma byc
           nowym poczatkiem listy lub jej drugim elementem. W przeicwnym
           przypadku wywolywna jest podfunkcja sort_into_list.

 * \param list - wskaznik na liste do ktorej dodajemy element.
 * \param to_add - wskaznik na element ktory ma byc dodany do listy.
 * \return wskaznik na liste z dodanym elementem.
 *
 */
static NODE_LIST* add_to_list(NODE_LIST* list, NODE* to_add) {

    if(list == NULL)
        return create_list_element(to_add);

    else if(list->node->number > to_add->number) {

        NODE_LIST* new_beginning = create_list_element(to_add);
        new_beginning->next = list;
        return new_beginning;
        }
    else
        sort_into_list(list, to_add);

    return list;
}

/** \brief Funkcja sprawdzajaca czy lista zawiera element.
           Funkcja sprawdza czy zadana lista zawiera element
           z numerem node_number. Wyowoluje sie rekurencyjnie.
 *
 * \param list - wskaznik na liste.
 * \param node_number - numer elementu ktorego szukamy.
 * \return TRUE jesli lista zawiera element.
           FALSE w przeciwnym przypadku.
 *
 */
static int list_contains(NODE_LIST* list, unsigned int node_number) {

    if(list == NULL)
        return FALSE;

    if(list->node->number == node_number)
        return TRUE;

    return list_contains(list->next, node_number);
}


/** \brief Funkcja usuwajaca element z listy.
           Funkcja usuwa z listy list element z node o identyfikatorze node_id.
           Funkcja zwraca wskaznik na nowo powstala liste. Jesli lista list
           jest pusta, nic nie robi, podobnie w przypadku gdy usuwany element
           nie znajduje sie w liscie, jednak w dzialanaiu programu zawsze bedzie
           on tam obecny.
 *
 * \param list - wskaznik na liste z ktorej mamy usunac element.
 * \param node_id - identyfikator elementu na liscie do usuniecia.
 * \return wskaznik na liste po usunieciu elementu.
 *
 */
static NODE_LIST* delete_from_list(NODE_LIST* list, unsigned int node_id) {

    if(list == NULL)
        return NULL;

    NODE_LIST* next_element = list->next;
    NODE_LIST* copy = list;
    //Przypadek gdy usuwamy pierwszy element z listy
    if(list->node->number == node_id) {
        free(list);
        return next_element;
    }

    //Przypadek gdy usuwamy cos z dalszej czesci listy
    while(next_element != NULL && next_element->node->number != node_id) {
        next_element = next_element->next;
        copy = copy->next;
    }

    copy->next = next_element->next;
    free(next_element);

    return list;
}


/** \brief Funkcja sprawdzajaca czy nie dochodzi do sprzezenia zwrotnego w ukladzie.
           Funkcja porownuje ojcow nowo tworzonego wezla z jego nowymi wejsciami.
           Jesli nowo tworzone wejscie jest gdzies wyzej tzn. jego wejscie zalezy
           od wyjscia nowow tworzonego wezla, to dochodzi do sprzezenia zwrotnego.
           Funkcja informuje wtedy o tym ze node o identyfikatorze node_id
           jest ojcem nowo tworzonego wejscia.
 *
 * \param fathers - wskanik na liste ojcow do porownywania.
 * \param father_id - identyfikator wejscia ktore moze tworzyc sprzeznie zwrotne.
 * \return TRUE jesli dochodzi do sprzezenia.
           FALSE w przeciwnym przypadku.
 *
 */
static int is_father(NODE_LIST* fathers, unsigned int father_id) {

    if(fathers == NULL)
        return FALSE;

    while(fathers != NULL) {

        if(fathers->node->number == father_id)
            return TRUE;

        int result = is_father(fathers->node->fathers, father_id);

        if(result)
            return TRUE;

        fathers = fathers->next;
    }

    return FALSE;
}

/** \brief Funkcja wyszukujaca w liscie elementow zdany element.
           Funkcja wyszukuje na liscie nodow node o identyfikatorze zgodnym z id.
           Jezeli taki element znajduje sie w liscie to jest zwracany wskaznik na niego.
           W przeciwnym przypadku zwracana jest wartosc NULL.
 *
 * \param list - wskaznik na liste do przeszukania.
 * \param id - identyfikator wezla ktory wyszukujemy.
 * \return wskaznik na znaleziony element jesli ten jest na liscie.
           NULL w przeciwnym przypadku.
 *
 */
static NODE* find_node_in_list(NODE_LIST* list, unsigned int id) {

    while(list != NULL && list->node->number != id)
        list = list->next;

    if(list == NULL)
        return NULL;

    return list->node;
}

/** \brief Funkcja tworzaca nowe polaczenie w ukladzie.
           Funkcja tworzy nowe polaczenie w ukladzie. Jest ona wywolywana
           w momencie gdy mamy pewnosc, ze tworzone polaczenie jest poprawne.
           Funkcja przeszukuje liste aktualnych wezlow i wejsc w celu znalezienia
           wezla ktory stanie sie wejsciem dla nowo tworzonego wezla. Jezeli taki
           wezel nie istnieje, tworzone jest nowe wejscie i dodawane do listy
           wejsc ukladu.
 *
 * \param id - identyfikator wezla ktory ma stac sie wejsciem.
 * \return wskaznik na nowe wejscie ukladu.
 *
 */
static NODE* find_connection(unsigned int id) {

    int contains = TRUE;

    NODE* new_enter = find_node_in_list(logic_board->all_nodes, id);

    //Przypadek ze nasz element bedzie nowy w ukladzie
    if(new_enter == NULL) {

        //Sprawdzamy czy moze nie laczymy wejsc
        new_enter = find_node_in_list(logic_board->enters, id);

        //Jesli nie to tworzymy nowy element do ukladu
        if(new_enter == NULL) {
            new_enter = create_empty_node(id, ENTER);
            contains = FALSE;
        }

    }
    //Jesli stoworzylismy nowy element do ukladu to go dodajemy
    if(!contains)
        logic_board->enters = add_to_list(logic_board->enters, new_enter);

    return new_enter;
}




/** \brief Funkcja dodajaca element do ukladu.
 *         Funkcja wywoluje sie wtedy, gdy wiadomo ze dodawane
           wezly do ukladu spelniaja warunki poprawnosi polecenia.
           Funkcja jest scaleniem innych podfunkcji i koordynuje ich
           wywolanie w zaleznosci od okreslonego typu wezla, ktory jest
           dodawany do ukladu.
 * \param to_add - wskaznik na dane wezla ktory ma byc dodany do ukladu.
 * \param type - typ wezla ktory ma byc dodany do ukladu.
 *
 */
static void add_enters_and_swap(unsigned int* to_add, int type) {

    //Znajdujemy i usuwamy nasze stare wejscie
    NODE* father = find_node_in_list(logic_board->enters, to_add[0]);
    logic_board->enters = delete_from_list(logic_board->enters, to_add[0]);

    //Przypadek ze nie ma naszego entera w liscie, wczesniej sprawdzamy
    //Poprawnosc paramterow do dodania w add_not_node i add_or_and_node
    if(father == NULL) {
        father = create_empty_node(to_add[0], type);
        logic_board->trunk = father;
    }

    NODE* new_enter = find_connection(to_add[1]);
    father->type = type;
    father->left_son = new_enter;
    logic_board->all_nodes = add_to_list(logic_board->all_nodes, father);
    new_enter->fathers = add_to_list(new_enter->fathers, father);

    //Jezeli mamy wezel and lub or to tworzymy jeszcze drugie wejscie dla niego
    if(type != NOT) {

        NODE* second_enter = find_connection(to_add[2]);
        father->right_son = second_enter;
        second_enter->fathers = add_to_list(second_enter->fathers, father);

    }

}



/** \brief Funkcja sprawdzajaca poprawnosc polaczenia dodawanego wezla.
 *         Funkcja sprawdza, czy dodany wezel nie duplikuje nazw wejsc, czy
           dodawany wezel ma unikalne id, czy nie ma sprzezenia zwrotnego.

 * \param to_add - wskaznik na dane wezla ktory ma byc dodany.
 * \param type - typ wezla ktory ma byc dodany do ukladu.
 * \return TRUE jesli wezel tworzy poprawne polaczenie.
           FALSE w przeciwnym przypadku.
 *
 */
static int correct_nodes_connection(unsigned int* to_add, int type) {

    if(to_add[0] == to_add[1])
        return FALSE;

    if(type != NOT && (to_add[1] == to_add[2] || to_add[0] == to_add[2]))
        return FALSE;

    //Pierwsze polaczenie zawsze bedzie dobre
    if(logic_board->trunk == NULL)
        return TRUE;

    //Mozemy dodawac cos tylko do wejscia, czyli jak nie ma wejscia o odpowiednim id to FALSE
    if(!list_contains(logic_board->enters, to_add[0]))
        return FALSE;

    NODE* our_enter = find_node_in_list(logic_board->enters, to_add[0]);

    //Sprzwdzamy czy nie wystepuje sprzezenie zwrotne
    if(is_father(our_enter->fathers, to_add[1]) || (type != NOT && is_father(our_enter->fathers, to_add[2])))
        return FALSE;

    //Jesli wszystko jest ok zwracamy TRUE
    return TRUE;
}



int add_node(unsigned int* to_add, int type) {

    if(logic_board == NULL)
        logic_board = create_board();

    if(!correct_nodes_connection(to_add, type))
        return ERR;

    add_enters_and_swap(to_add, type);

    return TRUE;
}


//Czysci liste bez zwalnianie elementow node
static void weak_NODE_LIST_free(NODE_LIST* list) {

    if(list == NULL)
        return;

    weak_NODE_LIST_free(list->next);
    free(list);
}


/** \brief Funkcja dealokujaca pamiec zajmowana przez liste wezlow.
 *         Funkcja dealokuje pamiec zajmowana przez liste.
           Zwalnia pamiec zajmowana przez wezly. Funkcja wywoluje sie
           rekurencyjnie.

 * \param element - wskaznik na element listy.
 *
 */
static void free_NODE_LIST(NODE_LIST* element) {

    if(element == NULL)
        return;

    free_NODE_LIST(element->next);
    weak_NODE_LIST_free(element->node->fathers);
    free(element->node);
    free(element);
}




void end_existing_board() {

    if(logic_board != NULL) {
        free_NODE_LIST(logic_board->all_nodes);
        free_NODE_LIST(logic_board->enters);
        free(logic_board);
    }

    logic_board = NULL;
}

//Funkcja wyznaczajaca dlugosc listy typu NODE_LIST
static int get_list_size(NODE_LIST* list) {

    if(list == NULL)
        return 0;

    return 1 + get_list_size(list->next);
}

static void insert_byte(unsigned int byte, NODE* node);



/** \brief Funkcja przekazujaca bit wezla jego ojcom.
           Funkcja przekazuje bit wezla wszystkim wejciom wezlow, do
           ktorych jest podlaczone jego wyjscie (zachodzi zaleznosc ojciec, syn).
 *
 * \param byte - bit ktory ma byc przekazany.
 * \param node - wskaznik na wezel ktory przekazuje na wyjscie but.
 *
 */
static void insert_byte_fathers(unsigned int byte, NODE* node) {

    NODE_LIST* fathers_tmp = node->fathers;

    if(fathers_tmp == NULL)
        logic_board->result = byte;

    while(fathers_tmp != NULL) {
        insert_byte(byte, fathers_tmp->node);
        fathers_tmp = fathers_tmp->next;
    }
}



/** \brief Funkcja wstawiajaca bit do wezla.
 *         Funkcja symuluje przeplyw bitu przez wezel z uwzglednieniem
           jego typu. Przesuwa bit tak dlugo w gore, jak mozliwe jest
           polaczenie (tzn. nie jestesmy pierwszym bitem w bramce or lub and).
           Wynik dzialania symulacji zamieszczony zostaje w strukturze
           logic_board w polu result.

 * \param byte - bit ktory ma byc wstawiony do wezla.
 * \param node - wezel przez ktory przepuszczany bedzie bit.
 *
 */
static void insert_byte(unsigned int byte, NODE* node) {

    //Jak jestesmy wejsciem to przekazujemy na gore
    if(node->type == ENTER) {
        insert_byte_fathers(byte, node);
        return;
    }

    //Jak jestesmy NOT to przekazujemy na gore i negujemy
    if(node->type == NOT) {
        insert_byte_fathers(!byte, node);
        return;
    }

    //Wstawiamy bit w odpowiednie miejsce do wezla
    if(node->input_byte == -1) {
        node->input_byte = byte;
        return;
    }

    int result = (node->type == AND ? node->input_byte && byte : node->input_byte || byte);
    node->input_byte = -1;
    insert_byte_fathers(result, node);
}

/** \brief Funkcja wprowadzajaca bity do wejsc ukladu logicznego.
 *         Funkcja wprowadza ciag bitow zadany przez uzytownika do ukladu
           logicznego. Wywolywana jest tylko w momencie, gdy wprowadzone
           dane sa poprawne.

 * \param bytes - wskaznik na tablice z bitami do zrobienia testu.
 *
 */
static void do_test(unsigned int* bytes) {

    NODE_LIST* enters_beginning = logic_board->enters;
    int index = 0;

    while(enters_beginning != NULL) {
        insert_byte(bytes[index], enters_beginning->node);
        index++;
        enters_beginning = enters_beginning->next;
    }
}


int test_board(unsigned int* bytes, int bytes_size) {

    //Przypadek gdy jest zla liczba bitow
    if(logic_board == NULL || get_list_size(logic_board->enters) != bytes_size)
        return ERR;

    do_test(bytes);

    return logic_board->result;
}


