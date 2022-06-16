#include <iostream>
#include <fstream>
#include <string>
#include <map>
using namespace std;


int main(void){

  //Se declaran las variables
  ifstream monfile;
  ofstream nouveaufile;
  char line[256];
  string fileA, fileB, val;
  string col[] = {"DOC_ALT DOCTO.","FECHA","CLIENTE",
                  "REFERENCIA","LOCAL","CANTIDAD",
                  "U.M.","VLR_BRUTO","DSCTO_PROM.",
                  "IMPUESTO","T O T A L","NUMREMISION",
                  "FECHAREMISION","SOLICITUDPEDIDO", "ORDENDECOMPRA"};
  map<string,string> info;

  //Se organiza la ubicacion y el nombre de los archivos
  cout << "Archivo a transformar: " << endl;
  cin >> fileA;
  cout << "Nombre del nuevo archivo: " << endl;
  cin >> fileB;

  fileA += ".txt";
  fileB += ".txt";
 //se abre los archivos / Comenza el proceso
  monfile.open(fileA, ios::in);
  nouveaufile.open("Archivos_Transformados/" + fileB, ios::out);
  //Se escriben las columnas dentro del documento
  if(nouveaufile.is_open()){
    for(int i = 0; i<15; i++){
      switch(i){
        case 14:
          nouveaufile << col[i] << endl;
          break;
        default:
          nouveaufile << col[i] << "#";
      }
    }
  }


  if(!monfile.is_open()){
      cout << "Error monfile not open" << endl;
  }else{
    while(monfile.getline(line,256)){
          if(line[0] == '#'){
            //Imprime la primera linea del texto
            if (!info.empty()) info.clear();  //limpia el mapa/diccionario para obtener las nuevas lineas

            for (int i = 1; i<monfile.gcount();i++){
              if(line[i] != ' ')val += line[i];;
              switch(i){
                case 15:
                  info.insert(pair<string,string>(col[0],val));
                  val="";
                  break;
                case 28:
                  info.insert(pair<string,string>(col[1],val));
                  val="";
                  break;
                case 46:
                  info.insert(pair<string,string>(col[2],val));
                  val="";
                  break;
              }

            }


          }else if(line[0]==' ' && line[1]!='-'){
            //imprime segunda linea que tiene los caracteres restantes del cliente
            if(line[1] == ' '){
              val = info[col[2]];
              info.erase(col[2]);
              for(unsigned int i = 30; i <=47; i++){ if(line[i] == ' ') continue; val += line[i];}
              info.insert(pair<string,string>(col[2],val));
              val = "";
            }else if(line[1] == 'R'){
              //imprime tercera linea de texto que contiene:
              for (int i = 0; i<monfile.gcount(); i++){
                if (line[i] == ' ') continue;
                val += line[i];
                if(i == 9) {info.insert(pair<string,string>(col[11],val)); val = "";} //  NÚMERO DE REMISIÓN
                if(i == 24) {info.insert(pair<string,string>(col[12],val)); val = "";}//  FECHA DE REMISIÓN
                if(i == 60) {info.insert(pair<string,string>(col[13],val)); val = "";}// SOLICITUD DE PEDIDO
                if(i == monfile.gcount() - 1) {info.insert(pair<string,string>(col[14],val)); val = "";}// ORDEN DE COMPRA
              }


            }else{
              //Obtiene desde la tercera linea hasta la anterior a iniciar una nueva (productos, costo tolal, etc)

              if(info.count(col[3]) > 0) {info.erase(col[3]);info.erase(col[4]);info.erase(col[5]);info.erase(col[6]);info.erase(col[7]);info.erase(col[8]);info.erase(col[9]);info.erase(col[10]);}
              for(int i = 0; i<monfile.gcount();i++){
                if (line[i] != ' ') val += line[i];

                if(i == 39){info.insert(pair<string,string>(col[3],val)); val = "";} //Ref
                if(i == 49){info.insert(pair<string,string>(col[4],val)); val = "";} //Local
                if(i == 60){info.insert(pair<string,string>(col[5],val)); val = "";} //cantidad
                if(i == 63){info.insert(pair<string,string>(col[6],val)); val = "";} //U.M
                if(i == 77){info.insert(pair<string,string>(col[7],val)); val = "";} //Val_bruto
                if(i == 89){info.insert(pair<string,string>(col[8],val)); val = "";} //Des_prom
                if(i == 101){info.insert(pair<string,string>(col[9],val)); val = "";} //impuesto
                if(i == monfile.gcount()-1){info.insert(pair<string,string>(col[10],val)); val = "";} // T O T A L
                }

              if(!nouveaufile.is_open()){
                cout << "Error nouveaufile not open" << endl;
              }else{
                for(int i = 0; i<15; i++){ //imprime la linea
                  switch(i){
                    case 14:
                      nouveaufile << info[col[i]] << endl;
                      break;
                    default:
                      nouveaufile << info[col[i]] << "#";
                  }
                }
              }


            }


          }



    }
    //Se cierran los archivos / Finaliza el proceso
    monfile.close();
    nouveaufile.close();
    cout << "----------" << endl;
    cout << "PROCESO EXITOSO" << endl;

  }
  return 0;
}
