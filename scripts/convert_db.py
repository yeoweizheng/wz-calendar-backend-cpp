import os
import sqlite3
import sys

old_dbfile = sys.argv[1]
old_conn = sqlite3.connect(old_dbfile)
old_cur = old_conn.cursor()

users = old_cur.execute('SELECT id, username, password FROM auth_user;').fetchall()
schedule_items = old_cur.execute('SELECT id, name, date, user_id, done, tag_id FROM schedule_scheduleitem;').fetchall()
tags = old_cur.execute('SELECT id, name, user_id FROM schedule_tag;').fetchall()

new_dbfile = 'newdb.sqlite3'
if os.path.exists(new_dbfile):
    os.remove(new_dbfile)

new_conn = sqlite3.connect(new_dbfile)
new_cur = new_conn.cursor()
new_cur.execute('CREATE TABLE user ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "username" varchar(150) NOT NULL UNIQUE, "password" varchar(128) NOT NULL)')
new_cur.execute('CREATE TABLE scheduleitem ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "name" varchar(512) NOT NULL, "date" date NOT NULL, "user_id" integer NOT NULL REFERENCES "user" ("id") DEFERRABLE INITIALLY DEFERRED, "done" bool NOT NULL, "tag_id" bigint NULL REFERENCES "tag" ("id") DEFERRABLE INITIALLY DEFERRED)')
new_cur.execute('CREATE TABLE "tag" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "name" varchar(100) NOT NULL, "user_id" integer NOT NULL REFERENCES "user" ("id") DEFERRABLE INITIALLY DEFERRED)')
new_cur.execute('CREATE INDEX "scheduleitem_tag_id" ON "scheduleitem" ("tag_id")')
new_cur.execute('CREATE INDEX "scheduleitem_user_id" ON "scheduleitem" ("user_id")')
new_cur.execute('CREATE INDEX "tag_user_id" ON "tag" ("user_id")')

for u in users:
    new_cur.execute(f'INSERT INTO user VALUES({u[0]}, "{u[1]}", "{u[2]}");')

for t in tags:
    new_cur.execute(f'INSERT INTO tag VALUES({t[0]}, "{t[1]}",{t[2]});')

for s in schedule_items:
    if s[5] == None:
        new_cur.execute(f'INSERT INTO scheduleitem VALUES({s[0]}, "{s[1]}", "{s[2]}", {s[3]}, {s[4]}, NULL);')
    else:
        new_cur.execute(f'INSERT INTO scheduleitem VALUES({s[0]}, "{s[1]}", "{s[2]}", {s[3]}, {s[4]}, {s[5]});')

new_conn.commit()