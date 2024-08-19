from django.shortcuts import render, redirect
from django.http import HttpResponse, JsonResponse
from django.shortcuts import render
import MySQLdb
import json
from datetime import datetime

# Create your views here.

TEMP_ITEMS = []
TEMP_USERS = []


def test(request):
    """
    Anchor for test.html
    """
    conn = MySQLdb.connect(host="127.0.0.1",
                           user="root",
                           passwd="mingtianshi20033",
                           db="ocean",
                           charset='utf8')

    with conn.cursor(cursorclass=MySQLdb.cursors.DictCursor) as cursor:
        cursor.execute("SELECT number, la, time, msg FROM oceandatabase_test")
        bolb = cursor.fetchall()
        return render(request, 'test.html', {'bolb': bolb})


def dashboard(request):

    conn = MySQLdb.connect(host="127.0.0.1",
                           user="root",
                           passwd="mingtianshi20033",
                           db="ocean",
                           charset='utf8')

    status = False
    with conn.cursor(cursorclass=MySQLdb.cursors.DictCursor) as cursor:
        try:
            cursor.execute(f"""SELECT title, content, time FROM oceandatabase_posts""")
            fetched = cursor.fetchall()

            # return render(request, 'dashboard.html', {"news": fetched})
        except:
            conn.rollback()

    bios = [
        {'name': 'Balaenoptera musculus', 'description': 'Mammal, Atlantic Ocean', 'value': 'Cs-137, 2.5 Bq/m3'},
        {'name': 'Carcharodon carcharias', 'description': 'Fish, Pacific Ocean', 'value': 'Sr-90, 2.5 Bq/m3'},
        {'name': 'Delphinus delphis', 'description': 'Mammal, Indian Ocean', 'value': 'I-131, 2.5 Bq/m3'},
        {'name': 'Chelonia mydas', 'description': 'Reptile, Pacific Ocean', 'value': 'Co-60, 2.5 Bq/m3'},
        {'name': 'Octopus vulgaris', 'description': 'Cephalopod, Atlantic Ocean', 'value': 'Cs-137, 2.5 Bq/m3'},
        {'name': 'Asterias rubens', 'description': 'Echinoderm, Indian Ocean', 'value': 'Sr-90, 2.5 Bq/m3'},
        {'name': 'Hippocampus kuda', 'description': 'Fish, Atlantic Ocean', 'value': 'I-131, 2.5 Bq/m3'},
        {'name': 'Aurelia aurita', 'description': 'Cnidarian, Pacific Ocean', 'value': 'Co-60, 2.5 Bq/m3'},
        {'name': 'Cancer pagurus', 'description': 'Crustacean, Atlantic Ocean', 'value': 'Cs-137, 2.5 Bq/m3'},
        {'name': 'Homarus americanus', 'description': 'Crustacean, Pacific Ocean', 'value': 'Sr-90, 2.5 Bq/m3'},
        {'name': 'Amphiprioninae ocellaris', 'description': 'Fish, Indian Ocean', 'value': 'I-131, 2.5 Bq/m3'},
        {'name': 'Manta birostris', 'description': 'Fish, Pacific Ocean', 'value': 'Co-60, 2.5 Bq/m3'},
        {'name': 'Xiphias gladius', 'description': 'Fish, Atlantic Ocean', 'value': 'Cs-137, 2.5 Bq/m3'},
        {'name': 'Lophius piscatorius', 'description': 'Fish, Pacific Ocean', 'value': 'Sr-90, 2.5 Bq/m3'},
        {'name': 'Strongylocentrotus purpuratus', 'description': 'Echinoderm, Indian Ocean', 'value': 'I-131, 2.5 Bq/m3'},
        {'name': 'Megaptera novaeangliae', 'description': 'Mammal, Atlantic Ocean', 'value': 'Co-60, 2.5 Bq/m3'},
        {'name': 'Orcinus orca', 'description': 'Mammal, Pacific Ocean', 'value': 'Cs-137, 2.5 Bq/m3'},
        {'name': 'Spheniscus demersus', 'description': 'Bird, Southern Ocean', 'value': 'Sr-90, 2.5 Bq/m3'},
        {'name': 'Zalophus californianus', 'description': 'Mammal, Pacific Ocean', 'value': 'I-131, 2.5 Bq/m3'},
        {'name': 'Odobenus rosmarus', 'description': 'Mammal, Arctic Ocean', 'value': 'Co-60, 2.5 Bq/m3'}
    ]

    import random
    random_sample = random.sample(bios, 7)


    return render(request, 'dashboard.html', {'bios': random_sample, "news": fetched})


def datatables(request):
    global TEMP_ITEMS
    conn = MySQLdb.connect(host="127.0.0.1",
                           user="root",
                           passwd="mingtianshi20033",
                           db="ocean",
                           charset='utf8')

    with conn.cursor(cursorclass=MySQLdb.cursors.DictCursor) as cursor:
        data = ""
        bolb = ""
        if request.method == 'POST':
            data = json.loads(request.body)

            print(data)

            nuclide = data['filter1']
            sample_type = data['filter2']
            start_date = data['start_date']
            end_date = data['end_date']
            depth_min = data['depth_min']
            depth_max = data['depth_max']

            start_date = datetime.strptime(start_date, '%d/%m/%Y').strftime('%Y-%m-%d')
            end_date = datetime.strptime(end_date, '%d/%m/%Y').strftime('%Y-%m-%d')

            print(start_date, end_date)

            cursor.execute(f"SELECT * FROM oceandatabase_rawdata where nuclide = '{nuclide}' AND sample_type = '{sample_type}' AND (sampling_depth >= '{depth_min}' AND sampling_depth <= '{depth_max}')")

            bolb = cursor.fetchall()
            TEMP_ITEMS = bolb
            print(len(bolb))
            return JsonResponse({"bolb": bolb})

        return render(request, 'datatables.html', {"bolb": bolb})


def datatablesAddItem(request):

    # 获取从前端发送的表单数据
    data = json.loads(request.body)
    nuclide = data['nuclide']
    sample_type = data['sampleType']
    sampling_depth = data['samplingDepth']
    latitude = data['altitude']
    longitude = data['longitude']
    temperature = data['temperature']
    sampling_date = data['samplingDate']
    activity_or_mda = data['activityOrMDA']
    additional_info = data['additionalInfo']
    print(nuclide)

    formatted_sampling_date = datetime.strptime(sampling_date, '%m/%d/%Y').strftime('%Y-%m-%d')


    conn = MySQLdb.connect(host="127.0.0.1",
                           user="root",
                           passwd="mingtianshi20033",
                           db="ocean",
                           charset='utf8')

    with conn.cursor(cursorclass=MySQLdb.cursors.DictCursor) as cursor:
        try:
            cursor.execute(f"""INSERT INTO ocean.oceandatabase_rawdata
            (latitude,
            longitude,
            sample_type,
            temperature,
            sampling_depth,
            nuclide,
            sampling_start_datetime,
            activity_or_mda,
            sample_note)
            SELECT
            '{latitude}' AS latitude,
            '{longitude}' AS longitude,
            '{sample_type}' AS sample_type,
            '{temperature}' AS temperature,
            '{sampling_depth}' AS sampling_depth,
            '{nuclide}' AS nuclide,
            '{formatted_sampling_date}' AS sampling_start_datetime,
            '{activity_or_mda}' AS activity_or_mda,
            '{additional_info}' AS sample_note""")
            conn.commit()

        except Exception as e:
            print("error", e)
            conn.rollback()


    response_data = {
        'status': 'success'
    }

    # 返回一个JSON响应，可以包含处理后的数据或其他信息
    return JsonResponse(response_data)



def datatablesDeleteItem(request):
    data = json.loads(request.body)
    conn = MySQLdb.connect(host="127.0.0.1",
                           user="root",
                           passwd="mingtianshi20033",
                           db="ocean",
                           charset='utf8')

    print(TEMP_ITEMS[int(data['ID'])]['id'])
    with conn.cursor(cursorclass=MySQLdb.cursors.DictCursor) as cursor:
        try:
            cursor.execute(f"""DELETE FROM oceandatabase_rawdata where id = {TEMP_ITEMS[int(data['ID'])]['id']}""")
            conn.commit()

        except:
            conn.rollback()

    response_data = {
        'status': 'success'
    }

    # 返回一个JSON响应，可以包含处理后的数据或其他信息
    return JsonResponse(response_data)


def datatablesModifyItem(request):
    data = json.loads(request.body)
    conn = MySQLdb.connect(host="127.0.0.1",
                           user="root",
                           passwd="mingtianshi20033",
                           db="ocean",
                           charset='utf8')

    id = TEMP_ITEMS[int(data['id'])]['id']
    field = data['field']
    target = data['target']

    with conn.cursor(cursorclass=MySQLdb.cursors.DictCursor) as cursor:
        try:
            cursor.execute(f"""UPDATE ocean.oceandatabase_rawdata SET {field} = {target} WHERE id = {id}""")
            conn.commit()
        except:
            conn.rollback()

    response_data = {
        'status': 'success'
    }

    # 返回一个JSON响应，可以包含处理后的数据或其他信息
    return JsonResponse(response_data)


def forgot_password(request):
    return render(request, 'forgot-password.html')


def forms_sample(request):
    return render(request, 'forms_sample.html')


def forms_source(request):
    conn = MySQLdb.connect(host="127.0.0.1",
                           user="root",
                           passwd="mingtianshi20033",
                           db="ocean",
                           charset='utf8')

    with conn.cursor(cursorclass=MySQLdb.cursors.DictCursor) as cursor:
        # data = json.loads(request.body)
        #
        # print(data)
        if request.method == 'POST':
            cursor.execute(f"SELECT * FROM oceandatabase_radioactivesource")
            bolb = cursor.fetchall()
            TEMP_USERS = bolb
            print(len(bolb))
            return JsonResponse({"bolb": bolb})

    return render(request, 'forms_source.html')


def AddSource(request):
    if request.method == 'POST':
        # 获取从前端发送的表单数据
        source_name = request.POST.get('source_name')
        nuclide = request.POST.get('nuclide')
        print(nuclide)
        add_date = request.POST.get('time')
        lg = request.POST.get('lg')
        at = request.POST.get('at')
        comment = request.POST.get('comment')
        country = request.POST.get('country')

        add_date = datetime.strptime(add_date, '%m/%d/%Y').strftime('%Y-%m-%d')

        conn = MySQLdb.connect(host="127.0.0.1",
                               user="root",
                               passwd="mingtianshi20033",
                               db="ocean",
                               charset='utf8')

        with conn.cursor(cursorclass=MySQLdb.cursors.DictCursor) as cursor:
            try:
                cursor.execute(f"""INSERT INTO ocean.oceandatabase_radioactivesource
                (source_name,
                nuclide,
                time,
                lg,
                at,
                status,
                ISO_country)
                SELECT
                '{source_name}' AS source_name,
                '{nuclide}' AS nuclide,
                '{add_date}' AS time,
                '{lg}' AS lg,
                '{at}' AS at,
                '{comment}' AS status,
                '{country}' AS ISO_country""")
                conn.commit()

            except Exception as e:
                print("error", e)
                conn.rollback()


        response_data = {
            'status': 'success'
        }

        # 返回一个JSON响应，可以包含处理后的数据或其他信息
        return JsonResponse(response_data)
    else:
        # 如果请求不是POST，返回错误信息
        return JsonResponse({'error': '只允许POST请求'}, status=400)


def SearchSource(request):
    bolb = {}
    print(request.POST)
    source_name = request.POST.get('source_name')
    print(source_name)
    conn = MySQLdb.connect(host="127.0.0.1",
                           user="root",
                           passwd="mingtianshi20033",
                           db="ocean",
                           charset='utf8')

    with conn.cursor(cursorclass=MySQLdb.cursors.DictCursor) as cursor:
        try:
            cursor.execute(f"""SELECT * FROM oceandatabase_radioactivesource where source_name = '{source_name}'""")
            conn.commit()
            bolb = cursor.fetchall()

        except Exception as e:
            print("error", e)
            conn.rollback()
    print(bolb)
    # 返回一个JSON响应，可以包含处理后的数据或其他信息
    return JsonResponse({'bolb': bolb})

def invoice(request):
    return render(request, 'invoice.html')


def lock(request):
    return render(request, 'lock.html')


def map(request):
    conn = MySQLdb.connect(host="127.0.0.1",
                           user="root",
                           passwd="mingtianshi20033",
                           db="ocean",
                           charset='utf8')

    with conn.cursor(cursorclass=MySQLdb.cursors.DictCursor) as cursor:
        data = ""
        bolb = ""
        if request.method == 'POST':
            # 接收POST请求中的JSON数据
            data = json.loads(request.body)
            print(data)

            nuclide = data['element']

            cursor.execute(f"SELECT * FROM oceandatabase_rawdata where nuclide = '{nuclide}' ")

            bolb = cursor.fetchall()

            print(len(bolb))

            # 如果是Ajax请求，返回JSON格式的数据
            return JsonResponse({"bolb": bolb})

    return render(request, 'map.html')


def messages(request):
    conn = MySQLdb.connect(host="127.0.0.1",
                           user="root",
                           passwd="mingtianshi20033",
                           db="ocean",
                           charset='utf8')

    status = False
    with conn.cursor(cursorclass=MySQLdb.cursors.DictCursor) as cursor:
        try:
            cursor.execute(f"""SELECT id, title, content, time FROM oceandatabase_waitingpool""")
            fetched = cursor.fetchall()

            return render(request, 'messages.html', {"data": fetched})
        except:
            conn.rollback()

    return render(request, 'messages.html', {"data": {}})


def reset_password(request):
    return render(request, 'reset-password.html')


def settings(request):
    if request.method == 'POST':
        data = json.loads(request.body)
        print(data)

        user_name = data['user_name']
        password = data['password']
        new_password = data['new_password']

        conn = MySQLdb.connect(host="127.0.0.1",
                               user="root",
                               passwd="mingtianshi20033",
                               db="ocean",
                               charset='utf8')

        status = False
        changed = False
        with conn.cursor(cursorclass=MySQLdb.cursors.DictCursor) as cursor:
            try:
                cursor.execute(f"""SELECT password FROM oceandatabase_register WHERE user_name = '{user_name}' """)
                fetched = cursor.fetchone()
                status = fetched['password'] == password
            except:
                conn.rollback()

            if status:
                try:
                    # print(f"""UPDATE ocean.oceandatabase_register SET password = '{new_password}' WHERE user_name = '{user_name}'""")
                    cursor.execute(f"""UPDATE ocean.oceandatabase_register SET password = '{new_password}' WHERE user_name = '{user_name}'""")
                    conn.commit()
                    changed = True
                except:
                    changed = False
                    conn.rollback()
            print({"status": status, 'changed': changed})

            return JsonResponse({"status": status, 'changed': changed})
    return render(request, 'settings.html')


def sign_in(request):
    if request.method == 'POST':
        data = json.loads(request.body)
        print(data)

        user_name = data['user_name']
        password = data['password']


        conn = MySQLdb.connect(host="127.0.0.1",
                               user="root",
                               passwd="mingtianshi20033",
                               db="ocean",
                               charset='utf8')

        status = False
        isAdmin = 0
        with conn.cursor(cursorclass=MySQLdb.cursors.DictCursor) as cursor:
            try:
                cursor.execute(f"""SELECT password, role FROM oceandatabase_register WHERE user_name = '{user_name}' """)
                fetched = cursor.fetchone()
                status = fetched['password'] == password
                if fetched['role'] == 1:
                    isAdmin = 1
                elif fetched['role'] == 2:
                    isAdmin = 2
            except:
                conn.rollback()

            print(isAdmin)
            return JsonResponse({"status": status, "isAdmin": isAdmin})

    return render(request, 'sign-in.html')


def sign_up(request):
    if request.method == 'POST':
        data = json.loads(request.body)
        print(data)

        user_name = data['user_name']
        user_mail = data['user_mail']
        password = data['password']

        conn = MySQLdb.connect(host="127.0.0.1",
                               user="root",
                               passwd="mingtianshi20033",
                               db="ocean",
                               charset='utf8')

        status = False
        with conn.cursor(cursorclass=MySQLdb.cursors.DictCursor) as cursor:
            try:
                cursor.execute(f"""SELECT COUNT(*) FROM oceandatabase_register WHERE user_name = '{user_name}' """)
                fetched = cursor.fetchone()['COUNT(*)']

                if fetched > 0:
                    status = False
                else:
                    status = True
                    cursor.execute(f"""INSERT INTO oceandatabase_register
                                    (user_name, password, email, role)
                                    SELECT
                                    '{user_name}' AS user_name,
                                    '{password}' AS password,
                                    '{user_mail}' AS email,
                                    1 AS role""")
                    conn.commit()
            except:
                conn.rollback()

            return JsonResponse({"status": status})

    return render(request, 'sign-up.html')


def single_message(request):
    return render(request, 'single-message.html')


def tasks(request):
    conn = MySQLdb.connect(host="127.0.0.1",
                           user="root",
                           passwd="mingtianshi20033",
                           db="ocean",
                           charset='utf8')

    status = False
    with conn.cursor(cursorclass=MySQLdb.cursors.DictCursor) as cursor:
        try:
            cursor.execute(f"""SELECT title, content, time FROM oceandatabase_posts""")
            fetched = cursor.fetchall()

            return render(request, 'tasks.html', {"data": fetched})
        except:
            conn.rollback()


    return render(request, 'tasks.html', {"data": {}})


def tasksAddPost(request):
    if request.method == 'POST':
        # 获取从前端发送的表单数据
        title = request.POST.get('title')
        content = request.POST.get('content')
        time = request.POST.get('time')

        print(title, content, time)

        conn = MySQLdb.connect(host="127.0.0.1",
                               user="root",
                               passwd="mingtianshi20033",
                               db="ocean",
                               charset='utf8')

        with conn.cursor(cursorclass=MySQLdb.cursors.DictCursor) as cursor:
            try:
                cursor.execute(f"""INSERT INTO ocean.oceandatabase_posts
                (title, content, time)
                SELECT
                '{title}' AS title,
                '{content}' AS content,
                '{time}' AS time""")
                conn.commit()

            except:
                conn.rollback()

        response_data = {
            'status': 'success'
        }

        # 返回一个JSON响应，可以包含处理后的数据或其他信息
        return JsonResponse(response_data)

def transactions(request):
    return render(request, 'transactions.html')


def users(request):
    global TEMP_USERS
    conn = MySQLdb.connect(host="127.0.0.1",
                           user="root",
                           passwd="mingtianshi20033",
                           db="ocean",
                           charset='utf8')

    with conn.cursor(cursorclass=MySQLdb.cursors.DictCursor) as cursor:
        # data = json.loads(request.body)
        #
        # print(data)
        if request.method == 'POST':

            cursor.execute(f"SELECT * FROM oceandatabase_register")
            bolb = cursor.fetchall()
            TEMP_USERS = bolb
            print(len(bolb))
            return JsonResponse({"bolb": bolb})

        return render(request, 'users.html')


def datatablesModifyItemUser(request):
    data = json.loads(request.body)
    conn = MySQLdb.connect(host="127.0.0.1",
                           user="root",
                           passwd="mingtianshi20033",
                           db="ocean",
                           charset='utf8')

    # print(TEMP_USERS)
    # print(int(data['id']))
    id = TEMP_USERS[int(data['id'])]['id']
    field = data['field']
    target = data['target']
    # print(id, field, target)

    with conn.cursor(cursorclass=MySQLdb.cursors.DictCursor) as cursor:
        try:
            print(f"""UPDATE ocean.oceandatabase_register SET {field} = {target} WHERE id = {id}""")
            cursor.execute(f"""UPDATE ocean.oceandatabase_register SET {field} = '{target}' WHERE id = {id}""")
            conn.commit()
        except Exception as e:
            print(e)
            conn.rollback()

    response_data = {
        'status': 'success'
    }

    # 返回一个JSON响应，可以包含处理后的数据或其他信息
    return JsonResponse(response_data)


def datatablesDeleteItemUser(request):
    data = json.loads(request.body)
    conn = MySQLdb.connect(host="127.0.0.1",
                           user="root",
                           passwd="mingtianshi20033",
                           db="ocean",
                           charset='utf8')
    # print(data)
    # print(TEMP_ITEMS[int(data['ID'])]['id'])
    # print(f"""DELETE FROM oceandatabase_register where id = {TEMP_USERS[int(data['ID'])]['id']}""")
    with conn.cursor(cursorclass=MySQLdb.cursors.DictCursor) as cursor:
        try:
            cursor.execute(f"""DELETE FROM oceandatabase_register where id = {TEMP_USERS[int(data['ID'])]['id']}""")
            conn.commit()
        except Exception as e:
            print(e)
            conn.rollback()

    response_data = {
        'status': 'success'
    }

    # 返回一个JSON响应，可以包含处理后的数据或其他信息
    return JsonResponse(response_data)


def loadUser(request):
    print(request.body)
    data = json.loads(request.body)
    name = data['name']
    conn = MySQLdb.connect(host="127.0.0.1",
                           user="root",
                           passwd="mingtianshi20033",
                           db="ocean",
                           charset='utf8')
    print(111)
    with conn.cursor(cursorclass=MySQLdb.cursors.DictCursor) as cursor:
        cursor.execute(f"SELECT user_name, email FROM oceandatabase_register where user_name = '{name}'")
        bolb = cursor.fetchall()
        return JsonResponse({'bolb': bolb})


def datatablesAddWaiting(request):
    data = json.loads(request.body)
    print(data)

    title = data['title']
    content = data['content']
    timer = data['time']

    conn = MySQLdb.connect(host="127.0.0.1",
                           user="root",
                           passwd="mingtianshi20033",
                           db="ocean",
                           charset='utf8')

    status = False


    with conn.cursor(cursorclass=MySQLdb.cursors.DictCursor) as cursor:
        try:
            cursor.execute(f"""INSERT INTO oceandatabase_waitingpool
                                    (title, content, time)
                                    SELECT
                                    '{title}' AS title,
                                    '{content}' AS content,
                                    '{timer}' AS time""")
            conn.commit()
        except Exception as e:
            print(e)
            status = True
            conn.rollback()

        return JsonResponse({"status": status})


def deleteWaiting(request):
    data = json.loads(request.body)
    id = data['id']
    conn = MySQLdb.connect(host="127.0.0.1",
                           user="root",
                           passwd="mingtianshi20033",
                           db="ocean",
                           charset='utf8')

    status = False
    print(id)

    with conn.cursor(cursorclass=MySQLdb.cursors.DictCursor) as cursor:
        try:
            cursor.execute(f"""DELETE FROM oceandatabase_waitingpool where id = {id}""")
            conn.commit()
        except Exception as e:
            print(e)
            status = True
            conn.rollback()

        return JsonResponse({"status": status})