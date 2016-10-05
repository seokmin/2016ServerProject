var express = require('express'),
    mysql = require('mysql'),
    ejs = require('ejs');

var connection = mysql.createConnection({
  host      : '13.94.44.159',
  port      : 3306,
  user      : 'jackblack',
  password  : '1234',
  database  : 'jackblack'
});

connection.connect(function(err) {
  if(err){
    console.error('mysql connection error');
    console.error(err);
    throw err;
  }
});

var app = express();

app.set('view engine', 'ejs');
app.set('views', __dirname + '/views');
app.use('/public', express.static(__dirname + '/public'));

app.get('/', function(req, res) {
  var query = connection.query('select username, chip from user order by chip desc', function(err, rows) {
    //console.log(rows);
    //res.json(rows);
    res.render('index', {
      users: rows
    });
  });
});

app.listen(5009);
