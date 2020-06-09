import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session, send_from_directory
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Query database for user's cash
    user_cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])[0]["cash"]
    total = user_cash

    # Query database for user's cash
    position = []
    stocks = db.execute("SELECT symbol, SUM(shares) FROM transactions WHERE userid = :user_id GROUP BY symbol HAVING SUM(shares) > 0", user_id=session["user_id"])
    for stock in stocks:
        stocksinfo = lookup(stock["symbol"])
        price = usd(stocksinfo["price"])
        shares = stock["SUM(shares)"]
        amount = usd(stocksinfo["price"] * shares)
        total +=  stocksinfo["price"] * shares
        position.append({"symbol": stocksinfo["symbol"], "name": stocksinfo["name"], "shares": shares, "price": price, "total": amount})

    # User reached route via GET (as by clicking a link or via redirect)
    return render_template("index.html", cash=usd(user_cash), position=position, total=usd(total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        response = lookup(symbol)

        if not symbol:
             return apology("must provide symbol", 403)

        elif not shares:
             return apology("must provide number of shares", 403)

        elif not response:
             return apology("symbol does not exist", 403)

        shares = int(shares)
        if shares < 1:
             return apology("invalid number of shares", 403)

        # Query database for user's cash
        user_cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])[0]["cash"]

        # Update transactions table
        price = round(response["price"], 2)
        if user_cash >= shares * price:
            db.execute("INSERT INTO transactions(userid, symbol, shares, price, timestamp) VALUES(?, ?, ?, ?, ?)",
                       session["user_id"], symbol, shares, price, datetime.now())
            db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash=user_cash-price*shares, id=session["user_id"])
        else:
            return apology("not enought money", 403)

        flash("Bought")
        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Query database for user's cash
    usertransactions = db.execute("SELECT symbol, shares, price, timestamp FROM transactions WHERE userid = :user_id", user_id=session["user_id"])

    # User reached route via GET (as by clicking a link or via redirect)
    return render_template("history.html", transactions=usertransactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        response = lookup(request.form.get("symbol"))
        if not response:
             return apology("symbol does not exist", 403)
        else:
            return render_template("quoted.html", name=response["name"], price=usd(response['price']), symbol=response["symbol"])

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        username = request.form.get("username")
        password = request.form.get("password")

        # Ensure username was submitted
        if not username:
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not password:
            return apology("must provide password", 403)

        # Ensure password match
        elif password != request.form.get("confirmation"):
            return apology("passwords don't match", 403)

        # Query database for username
        elif db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username")):
            return apology("username already registered", 403)

        # Insert user into database
        user_id = db.execute("INSERT INTO users(username, hash) VALUES(?, ?)", username, generate_password_hash(password))

        # Remember which user has logged in
        session["user_id"] = user_id

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        response = lookup(symbol)

        if not symbol:
             return apology("must provide symbol", 403)

        elif not shares:
             return apology("must provide number of shares", 403)

        elif not response:
             return apology("symbol does not exist", 403)

        shares = int(shares)
        if shares < 1:
             return apology("invalid number of shares", 403)

        # Query database for user's cash
        user_cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])[0]["cash"]

        # Update transactions table
        price = round(response["price"], 2)
        if db.execute("SELECT SUM(shares) FROM transactions WHERE symbol = :stock GROUP by userid = :user_id",
                      stock = symbol, user_id=session["user_id"])[0]["SUM(shares)"] - shares < 0:
            return apology("Not enough shares to sell", 403)
        else:
            db.execute("INSERT INTO transactions(userid, symbol, shares, price, timestamp) VALUES(?, ?, ?, ?, ?)",
                       session["user_id"], symbol, -shares, price, datetime.now())
            db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash=user_cash+price*shares, id=session["user_id"])

        flash("Sold")
        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        ownedstocks = db.execute("SELECT symbol FROM transactions WHERE userid = :user_id GROUP by symbol HAVING SUM(shares) > 0", user_id=session["user_id"])
        return render_template("sell.html", ownedstocks=ownedstocks)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
