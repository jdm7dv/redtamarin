/* -*- Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
/* vi: set ts=4 sw=4 expandtab: (add to ~/.vimrc: set modeline modelines=5) */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __avmplus_coff__
#define __avmplus_coff__


namespace avmplus
{
#if defined(DEBUGGER) && defined(AVMPLUS_IA32)
    namespace compiler
    {
        /** The magic number in COFF files signifying an i386 object file */
        const int I386MAGIC = 0x14c;

        /**
        * The header of a COFF file
        * @see Coff
        */
        typedef struct
        {
            unsigned short f_magic;         /**< magic number             */
            unsigned short f_nscns;         /**< number of sections       */
            unsigned long  f_timdat;        /**< time & date stamp        */
            unsigned long  f_symptr;        /**< file pointer to symtab   */
            unsigned long  f_nsyms;         /**< number of symtab entries */
            unsigned short f_opthdr;        /**< sizeof(optional hdr)     */
            unsigned short f_flags;         /**< flags                    */
        }
        CoffHeader_t;

        /**
        * If set, there is no relocation information in this file.
        * This is usually clear for objects and set for executables.
        */
        const int F_RELFLG  = 0x0001;

        /**
        * If set, all unresolved symbols have been resolved
        * and the file may be considered executable.
        */
        const int F_EXEC    = 0x0002;

        /**
        * If set, all line number information has been removed
        * from the file (or was never added in the first place).
        */
        const int F_LNNO    = 0x0004;

        /**
        * If set, all the local symbols have been removed
        * from the file (or were never added in the first place).
        */
        const int F_LSYMS   = 0x0008;

        /**
        * Indicates that the file is 32-bit little endian
        */
        const int F_AR32WR  = 0x0100;

        /**
        * A section header in the COFF file format
        * @see Coff
        */
        typedef struct
        {
            /** section name                     */
            char           s_name[8];

            /** physical address, aliased s_nlib */
            unsigned long  s_paddr;

            /** virtual address                  */
            unsigned long  s_vaddr;

            /** section size                     */
            unsigned long  s_size;

            /** file ptr to raw data for section */
            unsigned long  s_scnptr;

            /** file ptr to relocation           */
            unsigned long  s_relptr;

            /** file ptr to line numbers         */
            unsigned long  s_lnnoptr;

            /** number of relocation entries     */
            unsigned short s_nreloc;

            /** number of line number entries    */
            unsigned short s_nlnno;

            /** flags                            */
            unsigned long  s_flags;
        }
        SectionHeader_t;

        /**
        * If set, indicates that this section contains only executable code.
        */
        const int STYP_TEXT     = 0x0020;

        /**
        * If set, indicates that this section contains only initialized data.
        */
        const int STYP_DATA     = 0x0040;

        /**
        * If set, indicates that this section defines uninitialized data,
        * and has no data stored in the coff file for it.
        */
        const int STYP_BSS      = 0x0080;

    #pragma pack(2) // don't add trailing bytes
        /**
        * A symbol entry in the COFF file format
        * @see Coff
        */
        typedef struct
        {
            union
            {
                /** inline name */
                char str[8];
                struct
                {
                    unsigned long zero;
                    unsigned long offset;
                } table;
            } name;
            unsigned long   value;
            short           scnum;
            unsigned short  type;
            unsigned char   sclass;
            unsigned char   numaux;
        }
        SymbolEntry_t;
    #pragma pack()  // return to default

        /**
        * The Coff class writes out object files in the COFF format
        * (Common Object File Format)
        *
        * This is a useful tool for debugging the Compiler code generator
        * that translates AVM+ bytecode to native machine code.
        */
        class Coff
        {
        public:

            enum { NUM_SECTIONS = 64 };
            enum { NUM_SYMBOLS = 128 };

            Coff();
            void addSection(const char* ar, int numBytes, int startAddr, int endAddr);
            void addSymbol(const char* name, int value);
            void done();

        private:
            virtual ~Coff();
            void initHeader();
            char* write(char* dst, const char* src, int count);

            CoffHeader_t        header;
            SectionHeader_t*    sections[NUM_SECTIONS];
            SymbolEntry_t*      symbols[NUM_SYMBOLS];
        };
    }
#endif // DEBUGGER
}

#endif /* __avmplus_coff__ */
