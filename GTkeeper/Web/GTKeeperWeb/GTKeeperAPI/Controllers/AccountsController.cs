using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using GTKeeperAPI.Models;
using GTKeeperAPI.Models.Identity;

namespace GTKeeperAPI.Controllers
{
    [Route("api/[controller]")]
    public class AccountsController : Controller
    {

        public AccountsController(GTKeeperContext dbContext, GTKeeperUserManager account)
        {
           
        }
         




        [HttpGet("commit")]
        public IActionResult Commit(string imei)
        {
            return null;
        }

        [HttpGet("programacion")]
        public IActionResult Programacion2(string imei, string lup, string luc)
        {
            return null;
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


