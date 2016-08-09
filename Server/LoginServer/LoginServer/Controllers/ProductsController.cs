using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Web.Http;

namespace LoginServer.Controllers
{
    using LoginServer.Models;

    public class ProductsController : ApiController
    {
        Product[] products = new Product[]
        {
            new Product {id = 1, Name = "Tomato Soup", Category = "Groceries", Price = 1 },
            new Product {id = 2, Name = "Yo-yo", Category = "Toys", Price = 3.75M },
            new Product {id = 3, Name = "Hammer", Category = "Hardware", Price = 16.99M }
        };

        public IEnumerable<Product> GetAllProducts()
        {
            return products;
        }

        public HttpResponseMessage GetProductById(int id)
        {
            HttpResponseMessage response = null;
            var product = products.FirstOrDefault((p) => p.id == id);
            if(product == null)
            {
                response = new HttpResponseMessage(HttpStatusCode.NotFound);
            }
            else
            {
                response = Request.CreateResponse(HttpStatusCode.OK, product);
            }
            return response;
        }

        public IEnumerable<Product> GetProductsByCategory(string category)
        {
            return products.Where(
                (p) => string.Equals(p.Category, category,
                    StringComparison.OrdinalIgnoreCase));
        }
    }
}
