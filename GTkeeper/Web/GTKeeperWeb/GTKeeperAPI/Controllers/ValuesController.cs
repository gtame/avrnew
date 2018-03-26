using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;

namespace GTKeeperAPI.Controllers
{
    [Route("api/[controller]")]
    public class ValuesController : Controller
    {
        // GET api/values
        [HttpGet]
        public IEnumerable<string> Get()
        {
            return new string[] { "value1", "value2" };
        }

        // GET api/values/5
        [HttpGet("{id}")]
        public string Get(int id)
        {
            return "value";
        }



        [HttpPost("programacion")]
        public  IActionResult Programacion(IFormFile submitted)
        {
            //long size = files.Sum(f => f.Length);

            // full path to file in temp location
            var filePath = Path.GetTempFileName();

         
                if (submitted.Length > 0)
                {
                    using (var stream = new FileStream(filePath, FileMode.Create))
                    {
                    submitted.CopyTo(stream);
                    }
                }
            

            string[] lines = System.IO.File.ReadAllLines(filePath);
            foreach(string line in lines)
            {
                System.Diagnostics.Debug.WriteLine(line);

            }

            // process uploaded files
            // Don't rely on or trust the FileName property without validation.
            return Ok();
            //return Ok(new { count = 1, submitted.Length, filePath });
        }


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
    }
}
