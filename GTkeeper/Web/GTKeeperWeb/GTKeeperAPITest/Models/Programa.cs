using System;
using NUnit;
using NUnit.Framework;
using GTKeeperAPI.Models;

namespace GTKeeperAPITest.Models
{

    [TestFixture]
    public class Programa_test
    {
        [Test]
        [Category("ParsePrograma")]
        public void null_throwsException()
        {
            Assert.Throws<Exception>(() => Programa.ParsePrograma(null));
        }

        [TestCase("aaaaaaa8901234567")] //Con caracteres
        [TestCase("       8901234567")] //Con espacios
        [TestCase("1234567890123456722")] //Mas length
        [TestCase("12344567")]//Menos length
        [Category("ParsePrograma")]
        public void invalid_input_throwsException(string input)
        {
            Assert.Throws<Exception>(() => Programa.ParsePrograma(input));
        }


        [TestCase("123456789012345672")]//dia no valid
        [Category("ParsePrograma")]
        public void invalid_dias_throwsException(string input)
        {
            Assert.Throws<Exception>(() => Programa.ParsePrograma(input));
        }


        //SSDDDHHMMRRRRAAAA
        //SS   -> Sector
        //DDD  -> Dias que se ejecutara (enum)
        //HHMM -> Hora inicio ó Ejecución en el otro bloque
        //RRRR -> Tiempo de riego
        //AAAA -> Tiempo de abono

        [TestCase("01127120001200030")] //Mas length
        [TestCase("01001120001200130")]
        [Category("ParsePrograma")]
        public void parse_valid(string input)
        {
            Programa programa = Programa.ParsePrograma(input);
            Assert.True(input == programa.ToString());

        }


        //SSDDDHHMMRRRRAAAA
        //SS   -> Sector
        //DDD  -> Dias que se ejecutara (enum)
        //HHMM -> Hora inicio ó Ejecución en el otro bloque
        //RRRR -> Tiempo de riego
        //AAAA -> Tiempo de abono

        [TestCase("01127120001200030", 1, (Dia)127,  12 * 60, 120, 30)] //Mas length
        [TestCase("01001120001200130",1, (Dia)1,12 * 60 ,  120, 130)]
        [Category("ParsePrograma")]
        public void fields_valid(string input,int sector,Dia dia,int hora,int riego,int abono)
        {
            Programa programa = Programa.ParsePrograma(input);
            Assert.True(input == programa.ToString());

            Assert.True(programa.Sector == sector);
            Assert.True(programa.Dias == dia);
            Assert.True(programa.Hora == TimeSpan.FromMinutes(hora));
            Assert.True(programa.TiempoRiego == TimeSpan.FromMinutes(riego));
            Assert.True(programa.TiempoAbono == TimeSpan.FromMinutes(abono));

        }


        [TestCase("01127120001200030")] //Mas length
        [TestCase("01001120001200130")]
        [Category("ToString")]
        public void toString_valid(string input)
        {
            Programa programa = Programa.ParsePrograma(input);
            Assert.True(input == programa.ToString());

        }

    }
}
