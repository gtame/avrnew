using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using GTKeeperAPI.Models;
using GTKeeperAPI.Models.Identity;
using Microsoft.EntityFrameworkCore;


namespace GTKeeperAPI.Controllers
{
    [Route("api/[controller]")]
    public class ValuesController : Controller
    { 
        private readonly GTKeeperContext _dbContext;

        public ValuesController(GTKeeperContext dbContext)
        {
            _dbContext = dbContext;
      



    }
         

        //Obtiene 
        // GET api/values/5
        [HttpGet("{imei}")]
        public Device GetInfo(string imei)
        {
  
   
            return _dbContext.Devices.Include(d => d.Programas).First(d => d.Imei == imei);
         
        }


        [HttpGet("commit")]
        public async Task<IActionResult> Commit(string imei)
        {
            //Obtenemos el device
            Device device = GetInfo(imei);
            if (device == null)
                return NotFound($"Dispositivo no encontrado {imei}");

            device.CommitChanges();

            await _dbContext.SaveChangesAsync();

            return Ok();
        }

        [HttpGet("programacion")]
        public IActionResult GetProgramacion(string imei, string lup="", string luc="")
        {
            string result = string.Empty;
            //Obtenemos el device
            Device device = GetInfo(imei);
            if (device == null)
                return NotFound($"Dispositivo no encontrado {imei}");

       
            //Si tiene alguna salida activa la desactivamos - Forzandolas
            device.ClearSalidas();

            //Fijamos la hora de la ultima sincronizacion
            device.LastSync = System.DateTime.Now;

            //Chequeamos si esta pdte de sincronizar la config
            if (device.IsPendingConfig(luc))
                result += "\r\n" + device.ToString();

            //Chequeamos si esta pdte de sincronizar la programación
            if (device.IsPendingProgram(lup))
            {
                foreach (var programa in device.Programas)
                {
                  //  if (!programa.IsEmpty())
                    result += "+P:" + programa.ToString() + "\r\n";
                }
                    
            }


            return Ok(result);
        }


        [HttpPost("programacion")]
        public async Task<IActionResult> UpdateProgramacionAsync(string imei,string lup,string luc,IFormFile submitted)
        {
            string result = string.Empty;
             
            Device device = GetInfo(imei);
            if (device == null)
                return NotFound($"Dispositivo no encontrado {imei}");


            //Obtiene el archivo en el metodo post
            var filePath = Path.GetTempFileName();
            if (submitted!=null && submitted.Length > 0)
            {
                using (var stream = new FileStream(filePath, FileMode.Create))
                {
                   await submitted.CopyToAsync(stream);
                }
            }
            else
                return NotFound($"No hay disponible información de archivo");
            


            //Parseamos el archivo entrante
            string[] lines = System.IO.File.ReadAllLines(filePath);
            if (!device.ParseFile(lines,lup,luc))
                result = "+R:E\r\n";
            else
                result = "+R:O\r\n";

             device.CommitChanges();


          _dbContext.SaveChanges();


      ////Chequeamos si esta pdte de sincronizar la config
      if (device.IsPendingConfig(luc))
        result +=  "+C:" + device.ToString() + "\r\n";

      ////Chequeamos si esta pdte de sincronizar la programación
      if (device.IsPendingProgram(lup))
        foreach (var program in device.Programas)
        {
          if (!program.IsEmpty())
            result += "+P:" + program.ToString() + "\r\n";
        }


      //Todo recibido OK
      //return Ok("+R:O\r\n");
      //REcibido Error! :(
      //return Ok("+R:E\r\n");

      //Recibido y pasamos configuracion
      //return Ok("+R:O\r\n+C:12765331679911111215\r\n");



      //Recibido y pasamos programacion
      //return Ok("+R:O\r\n+C:12765331679911111215\r\n+D:O\r\n");


      //string programas = "+P:" + program.ToString() + "\r\n";
      return Ok(result );

            
            //return Ok("+R:O\r\n+C:12765331679911111215\r\n"+"");
            //return Ok(new { count = 1, submitted.Length, filePath });
        }

        /*
        // POST api/values
        [HttpPost]
        public void Post([FromBody]string value)
        {
        }

        // PUT api/values/5
        [HttpPut("{id}")]
        public void Put(int id, [FromBody]string value)
        {
        }

        // DELETE api/values/5
        [HttpDelete("{id}")]
        public void Delete(int id)
        {
        }

    */
    }
}


