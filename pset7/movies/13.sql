SELECT DISTINCT(name) FROM people JOIN stars ON people.id = person_id JOIN (SELECT movie_id AS mid FROM movies JOIN stars ON movies.id = movie_id JOIN people ON people.id = person_id WHERE name = "Kevin Bacon" AND birth = 1958) ON stars.movie_id = mid WHERE name != "Kevin Bacon"