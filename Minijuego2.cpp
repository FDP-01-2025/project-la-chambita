#include <iostream>
using namespace std;

int main()
{

    int maxIntentos = 2, intentos = 0;
    char opcion, respuesta = 'b';

    cout << "   Resuelve el ejercicio matematico  " << endl;
    cout << "      6 / 2 ( 1 + 2 )     " << endl;
    cout << " a. 1                 b. 9            " << endl;
    cout << " c. 6                 d. 0            " << endl;

    while (intentos < maxIntentos)
    {
        cout << "Elige una opcion (a, b, c, d): ";
        cin >> opcion;

        if (opcion == 'b')
        {
            cout << " Respuesta correcta:  " << opcion << endl;
            break; // Sale del ciclo si la opción es válida
        }
        else
        {
            cout << "Opcion invalida. ";
            intentos++;

            if (intentos < maxIntentos)
            {
                cout << "Intentalo de nuevo." << endl;
            }
            else
            {
                cout << " Has superado el numero maximo de intentos. La respuesta correcta era: " << respuesta << endl;
            }
        }
    }

    return 0;
}