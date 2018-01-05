/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LP_Leitura.h
 * Author: Rebeca
 *
 * Created on 8 de Novembro de 2017, 10:22
 */

/* 
 * File:   LP_Leitura_Prototype.h
 * Author: Joaquim
 *
 * Created on 7 de Janeiro de 2013, 3:00
 */

#ifndef LP_LEITURA_H
#define	LP_LEITURA_H

#ifdef	__cplusplus
extern "C" {
#endif

    void readShort(short *const value, const short minValue, 
            const short maxValue, char const* const message);
    
    void readInt(int *const value, const int minValue, 
            const int maxValue, char const* const message);
    
    void readLong(long *const value, const long minValue, 
            const long maxValue, char const* const message);
    
    void readFloat(float *const value, const float minValue, 
        const float maxValue, char const* const message);   
    
    void readDouble(double *const value, const double minValue, 
            const double maxValue, char const* const message);   

    void readChar(char *const value, char const* const message);
    
    int readString(char *const value, const unsigned int size, char const* const message);
    
    void readBool(int *const value, char const* const message);
    

#ifdef	__cplusplus
}
#endif

#endif	/* LP_LEITURA_H */



