#!C:\Users\DaltonLaptop\AppData\Local\Programs\Python\Python38-32\python.exe

print("")
print("JSON Created")
import json

data = {}
data['students'] = []
data['students'].append({
    'student_major': 'Web Development',
    'student_id': '332443',
    'student_gpa': '3.6',
	'student_courses': ['WDV101','WDV131', 'WDV105']
})
data['students'].append({
    'student_major': 'Web Development',
    'student_id': '332123',
    'student_gpa': '3.9',
	'student_courses': ['WDV321','WDV121', 'WDV131']
})
data['students'].append({
    'student_major': 'Photography',
    'student_id': '331227',
    'student_gpa': '3.0',
	'student_courses': ['CRS205','WDV131', 'SDV108']
})

with open('students.json', 'w') as outfile:
    json.dump(data, outfile)