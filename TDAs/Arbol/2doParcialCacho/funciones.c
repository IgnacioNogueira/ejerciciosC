#include "funciones.h"


int leerYMostrarArchivo(const char *nombreArchivo)
{
    FILE *fp;
    tProducto prod;

    fp = fopen(nombreArchivo,"rb");

    if(!fp)
    {
        return -2;
        printf("\nNo se pudo mostrar el archivo maestro");
    }

    fread(&prod,sizeof(tProducto),1,fp);

    while(!feof(fp))
    {
        mostrarArchivoMaestro(&prod);
        fread(&prod,sizeof(tProducto),1,fp);
    }

    printf("\n\n");
    fclose(fp);
    return TODO_BIEN;
}

void mostrarArchivoMaestro(tProducto *prod)
{
    printf("%2ld %-*.*s %02d/%02d/%04d %3d %8.2f\n",
           prod->idProducto,
           (int)sizeof(prod->descripcion)-1,
           (int)sizeof(prod->descripcion)-1,
           prod->descripcion,
           prod->fecUltAct.dia,
           prod->fecUltAct.mes,
           prod->fecUltAct.anio,
           prod->cantidad,
           prod->precioUnitario
          );
}


void mostrarIndice (const void * info)
{
    tIndice *ind = (tIndice *)info;
    printf("\n-ID: %d\n",ind->clave);
    printf("\n-POS: %d\n",ind->pos);
}

void recorrerArbolInOrderBasico(tArbolBinBusq *p,void (*accion)(const void *))
{
    if(!*p)
        return;
    recorrerArbolInOrderBasico(&(*p)->izq,accion);
    accion((*p)->info);
    recorrerArbolInOrderBasico(&(*p)->der,accion);

}


///Ejercicio 1

int cmp_ent(const void *v1,const void *v2)
{
    return *((int*)v1)-*((int*)v2);
}

int cmp_arbol_nov(const void *v1,const void *v2)
{
    return ((tIndice*)v1)->clave - *((long*)v2); //estoy errandole ac�
}

int cmp_ind(const void *v1,const void *v2)
{
    return ((tIndice*)v1)->clave - ((tIndice*)v2)->clave;
}

void imprimir(void *info,unsigned tam,unsigned n,void * params)
{
    int *i = (int *) info;
    printf("-%d-",*i);
}


/// 1 - Esta funci�n es para cargarlo como venga del archivo, probablemente desordenado
int cargarIndiceArbolArch(const char *nombreArchivo,tArbolBinBusq *p,
                          int(*cmp)(const void *,const void*))
{
    int i = 0;
    tProducto prod;
    tIndice ind;
    FILE *fp;

    fp = fopen(nombreArchivo,"rb");

    if(!fp)
    {
        printf("\nNo se pudo abrir el archivo %s",nombreArchivo);
        return -1;
    }

    fread(&prod,sizeof(tProducto),1,fp);

    while(!feof(fp))
    {
        ind.clave = prod.idProducto;
        ind.pos = i;
        insertarEnArbBinBusqRec(p,&ind,sizeof(tIndice),cmp);
        fread(&prod,sizeof(tProducto),1,fp);
        ++i;
    }

    fclose(fp);
    return TODO_BIEN;
} //arbol queda desbalanceado


///2 - guarda el arbol que tiene los registros del archivo, en un nuevo archivo temporal en modo inorder. D
///Despues se prepara un arbol balanceado

int guardarArbolGeneradoArchTemp(tArbolBinBusq *p,void (*accion)(const void *dato,void *contexto))
{
    FILE *temp = fopen("temp.idx","wb");

    if(!temp)
        return -3;

    inOrderArchivos(p,accion,temp);
    fclose(temp);
    return TODO_BIEN;
}

void guardarRegIndiceArchivo(const void *dato, void *contexto)
{
    fwrite(dato,sizeof(tIndice),1,(FILE*)contexto);
}

void inOrderArchivos(tArbolBinBusq *p,void (*accion)(const void *dato, void *contexto),void *param)
{

    if(!*p)
        return;

    inOrderArchivos(&(*p)->izq,accion,param);
    accion((*p)->info,param); //va a guardar el reg indice en el archivo
    inOrderArchivos(&(*p)->der,accion,param);

}

///3 - Despues de esta funcion queda SOLO EL ARBOL, el archivo temporal se borra

int cargarArbolArchivoINDTemp(tArbolBinBusq *p,int (*cmp)(const void *,const void *))
{
    int cantReng;

    FILE *fp = fopen("temp.idx","rb");

    if(!fp)
        return 0;

    fseek(fp,0L,SEEK_END);
    cantReng = ftell(fp)/sizeof(tIndice);
    cantReng = cargarArbolArchivoINDTemp_Int(p,0,cantReng-1,fp,cmp);

    fclose(fp);
    //unlink("temp.idx");
    return cantReng;
}

int cargarArbolArchivoINDTemp_Int(tArbolBinBusq *p,int li,int ls,FILE* archivo,
                                  int (*cmp)(const void *,const void *))
{
    int m = (li + ls)/2;
    tIndice ind;

    if(li > ls)
        return TODO_BIEN;

    fseek(archivo,m*sizeof(tIndice),SEEK_SET);
    fread(&ind,sizeof(tIndice),1,archivo);

    if(!insertarEnArbBinBusqRec(p,&ind,sizeof(tIndice),cmp))
        return SIN_MEM;

    return cargarArbolArchivoINDTemp_Int(&(*p)->izq,li,m-1,archivo,cmp)
           +  cargarArbolArchivoINDTemp_Int(&(*p)->der,m+1,ls,archivo,cmp);
}

///Ejercicio 2

void trozarCampoLongitudVariable(char *s,tNovedad *nov)
{
    char *aux = strchr(s,'\n');
    *aux = '\0';

    aux = strrchr(s,'|');
    sscanf(aux+1,"%f",&nov->precioUnitario);
    *aux = '\0';

    aux = strrchr(s,'|');
    sscanf(aux+1,"%c",&nov->tipoMovimiento);
    *aux = '\0';

    sscanf(s,"%ld",&nov->idProducto);
}

void mostrarNovedad(const void *dv)
{
    tNovedad *d = (tNovedad *)dv;

    printf("%2ld %c %8.2f\n",
           d->idProducto,
           d->tipoMovimiento,
           d->precioUnitario
          );
}

///Ejercicio 4

int ejercicio4(const char *archivoEstadosFinan, tCola *col,
               const char *archivoInforme,tArbolBinBusq *p,
               int (*cmp)(const void *,const void*))
{

    FILE *fpEF,*fpI;
    tProducto prod;
    tNovedad nov;
    tIndice ind;
    int cantAct = 0;


    fpEF = fopen(archivoEstadosFinan,"r+b"); //es el cuentas.dat que en realidad lo nombr� de otra manera xd

    if(!fpEF)
    {
        printf("\nNo se pudo acceder al archivo %s\n",archivoEstadosFinan);
        return -5;
    }

    fpI = fopen(archivoInforme,"wb");

    if(!fpI)
    {
        fclose(fpEF);
        printf("\nNo se pudo acceder al archivo %s\n",archivoInforme);
        return -6;
    }

    while(sacarEnColaCir(col,&nov,sizeof(tNovedad))) //la busqueda secuencial no est� permitida en el �ndice
    {
        fread(&prod,sizeof(tProducto),1,fpEF); //leo esa posici�n
        buscarIndArbolBinBusq(p,&prod,&ind,sizeof(tIndice),cmp);
        fseek(fpEF,(ind.pos * ((int)sizeof(tProducto))),SEEK_CUR);///est� bien porque va a mover la pos del indice dentro de ese archivo
        //estoy ingresando al archivo a actualizar x en vez del novedades

        if(prod.idProducto == nov.idProducto)
        {
            if(A_MAYUS(nov.tipoMovimiento) == 'I')
            {
                prod.precioUnitario+=nov.precioUnitario;
                fseek(fpEF,-1L*(long)sizeof(tProducto),SEEK_CUR);
                fwrite(&prod,sizeof(tProducto),1,fpEF);
                fseek(fpEF,0L,SEEK_CUR);
                ++cantAct;
            }
            else if(A_MAYUS(nov.tipoMovimiento) == 'E')
            {
                prod.precioUnitario-=nov.precioUnitario;
                fseek(fpEF,-1L*(long)sizeof(tProducto),SEEK_CUR);
                fwrite(&prod,sizeof(tProducto),1,fpEF);
                fseek(fpEF,0L,SEEK_CUR);
                ++cantAct;
            }
        }
        else
        {
            fwrite(&nov,sizeof(tNovedad),1,fpI);
        }

        rewind(fpEF);
    }

    fclose(fpEF);
    fclose(fpI);
    return cantAct;
}


