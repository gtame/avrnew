using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace GTKeeperAPI.Models
{
    public class GTKeeperContext : DbContext
    {
        public GTKeeperContext()
        {

        }

        public DbSet<Device> Devices { get; set; }
        public DbSet<Log> Logs { get; set; }
        public DbSet<Programa> Programas { get; set; }
        public DbSet<Salida> Salidas { get; set; }

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
        }

        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
        {
            optionsBuilder.UseSqlServer(@"Server=.;Database=SchoolDB;Trusted_Connection=True;");
        }

    }
}
