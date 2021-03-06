// -----------------------------------------------------------
// File: ESC_StandardSCI_Protocal.h
//
// Discription: Defines standard SCI protocal for ESC
// development.
//
// -----------------------------------------------------------
// Release Date: 2018-08-24;
// Authour: Steven Liu;
// -----------------------------------------------------------
#ifndef __ESC_StandardSCI_Protocal_H
#define __ESC_StandardSCI_Protocal_H

 # include "ESC_Device.h"
 # include "ESC_StandardSCI_DataTypeDef.h" 
//
// 1st.protocol definition 
//   .........................................................
//   a.frame:
//     . . . . . . . . . . . . . . . . . . . . . . . . . . . .
//     stop bits -- 1
//     parity    -- no parity
//     char-bits -- 8
//
//   b.package:
//     . . . . . . . . . . . . . . . . . . . . . . . . . . . .
//     frame-bytes -- 5
//
//     lowest address ��
//     1st-frame ---- package head
//     2nd-frame ---- lower 8 bits of data (0x00 to 0xFF)
//     3rd-frame ---- higher 8 bits of data (0x00 to 0xFF)
//     4th-frame ---- data type
//     5th-frame ---- CRC residue (0x00 to 0xFF)
//     highest address ��
//
//   c.CRC residue generating mathod
//     . . . . . . . . . . . . . . . . . . . . . . . . . . . .
//     G(7) = 2^7+2^6+2^1+1
//          = StdSci_CRC
//
//   d.baud rate -- 115200
//
// 2nd.macros
//   .........................................................
     # define StdSci_CharBits 8      // character bits
     # define StdSci_FrameBytes 5    // number of frames
     # define StdSci_PackHead 0xAA   // package head
     # define StdSci_CRC 0x00C3
     # define StdSci_RxBuffLength 32
		 # define StdSci_TX_OverRun 1
//
// 3rd.types
//   .........................................................
//   package structure defining frames
     typedef struct
     {
       volatile u16 PackHead; // head
       volatile u16 DataLwr;  // lower 8 bits of data
       volatile u16 DataHghr; // higher 8 bits of data
       volatile u16 DataType; // data type
       volatile u16 CrcRsd;   // CRC residue
     }STDSCI_FRAME;

//
//   package union
     typedef union
     {
       volatile u16 buffer[StdSci_FrameBytes];    // indexed array
       STDSCI_FRAME frame; // structured frames
     }STDSCI_PACK;

     struct STDSCI_PACK_TX
     {
       STDSCI_PACK pck;
			 Boole overrun;
       u32 tick;
			 struct STDSCI_PACK_TX* ptr_next_pck;
       volatile u8 index;
			 volatile Boole flag_pck_renewed;
     };
//   
//   Standard SCI Transmitting Block Sturcture
//   ...........
     typedef struct
     {
			 volatile u16 tick;

			 volatile Boole flag_TC;

       struct STDSCI_PACK_TX* thread_ptx_ptr;

       struct STDSCI_PACK_TX pck_0x00;

       struct STDSCI_PACK_TX pck_0x01;
       struct STDSCI_PACK_TX pck_0x02;
       struct STDSCI_PACK_TX pck_0x03;
       struct STDSCI_PACK_TX pck_0x04;

       struct STDSCI_PACK_TX pck_0x10;
       struct STDSCI_PACK_TX pck_0x11;

       struct STDSCI_PACK_TX pck_0x80;
       struct STDSCI_PACK_TX pck_0x81;
			 struct STDSCI_PACK_TX pck_0x82;

       struct STDSCI_PACK_TX pck_0x90;
       struct STDSCI_PACK_TX pck_0x91;
       struct STDSCI_PACK_TX pck_0x98;

       struct STDSCI_PACK_TX pck_0xA0;
       struct STDSCI_PACK_TX pck_0xA1;
       struct STDSCI_PACK_TX pck_0xA2;
       struct STDSCI_PACK_TX pck_0xA3;
       struct STDSCI_PACK_TX pck_0xA4;
       struct STDSCI_PACK_TX pck_0xA5;

       struct STDSCI_PACK_TX pck_0xB0;
       struct STDSCI_PACK_TX pck_0xB1;
       struct STDSCI_PACK_TX pck_0xB2;

       struct STDSCI_PACK_TX pck_0xC0;
       struct STDSCI_PACK_TX pck_0xC1;

       struct STDSCI_PACK_TX pck_0xF0;
       struct STDSCI_PACK_TX pck_0xF1;
       struct STDSCI_PACK_TX pck_0xF2;
       struct STDSCI_PACK_TX pck_0xF3;
     } STDSCI_BLOCK_TX;
		 
     typedef enum {Idle, Receiving}StdSci_RX_State;
//
//   package struct with a counter for receiving buffer
     typedef struct
     {
       STDSCI_PACK pck;
       StdSci_RX_State state;
       volatile u16 offset;
     }STDSCI_PACK_BUFF;
//
// 4th.function prototypes
//   .........................................................
//     void StdSci_Send(USART_TypeDef* USARTx, u16          \
//		      frm_DataType, u16 frm_Data);
//     void StdSci_Receive(USART_TypeDef* USARTx);
//     STDSCI_PACK_RX StdSci_Read(void);
//     void InitVrbls_StdSci (void);

     u16 StdSci_CrcRsdGnrtr(STDSCI_PACK pck);
		
     void STDSCI_BLOCK_TX_Init(void);
     void STDSCI_BUFF_RX_Init(void);
//     void STDSCI_FRAME_CHIN_RX_Init(void);

     Boole SET_STDSCI_PACK_TX(struct STDSCI_PACK_TX* ptx_ptr, u16 frm_Data);
     void SET_STDSCI_PACK_32_TX_OVERRUN( struct STDSCI_PACK_TX* ptx_L16_ptr, \
                                         struct STDSCI_PACK_TX* ptx_H16_ptr, \
                                         u32 frm_Data);

     void SET_STDSCI_PACK_32_TX_OVERRUN_INTERVAL( struct STDSCI_PACK_TX* ptx_L16_ptr, \
                                                  struct STDSCI_PACK_TX* ptx_H16_ptr, \
                                                  u32 frm_Data, u32 interval);

     void SET_STDSCI_PACK_TX_OVERRUN(struct STDSCI_PACK_TX* ptx_ptr, u16 frm_Data);
     void SET_STDSCI_PACK_TX_OVERRUN_INTERVAL(struct STDSCI_PACK_TX* ptx_ptr, u16 frm_Data, u32 interval);

     void StdSci_Send_in_ISR(USART_TypeDef* USARTx);		 
     void StdSci_Receive_in_ISR(USART_TypeDef* USARTx);

     extern STDSCI_BLOCK_TX block_tx; //2018.11.16 temp
     extern STDSCI_PACK_BUFF pack_rx_buff; //2018.11.22
		 
#endif /*#ifndef __ESC_StandardSCI_Protocal_H*/ 
// no more.
// ...........................................................

