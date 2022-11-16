import cv2
from PIL import Image
import matplotlib.pyplot as plt
import re
import easyocr
import pymysql
import pandas as pd
from util import car_time, result_plate, webcam, calc_price, signal

def extro_main():
    signal('COM9', 115200, 'out_1', 'utf-8')
    
    webcam(2, 'outro_image')


    img_path = './image/outro_image/img.png'

    img = cv2.imread(img_path)

    with open('obj.names', 'r') as f:
        classes = f.read().splitlines()

    #cfg 파일에 batch, subdivisions 는 둘 다 1이어야함

    net = cv2.dnn.readNetFromDarknet('yolov4-tiny-custom.cfg', 'yolov4-tiny-custom_best.weights')

    model = cv2.dnn_DetectionModel(net)
    model.setInputParams(scale=1 / 255, size=(416, 416), swapRB=True)

    try : 
        classIds, scores, boxes = model.detect(img, confThreshold=0.6, nmsThreshold=0.4)

        clss = classIds.tolist()
        score = scores.tolist()

        # 주차딱지 변수 값 지정
        # Object Detection 했을 때 주차딱지가 Detection 안 되면 class_num_y = '0'값을 가짐
        # 주차딱지가 Detection 되면 class_num_y는 '3' 값을 가짐 (아래에 있는 if 문)

        class_num_y = '0'

        reader = easyocr.Reader(['ko'])

        # # 검출된 bbox 중, 클래스가 0(norm), 1(elec), 3(yellow) 이면 이미지 출력

        for i in range(len(clss)):
            box = boxes[i].tolist()
            w = box[0]
            h = box[1]
            x = box[2]
            y = box[3]
            bbox = img[h:y+h, w:x+w]


            if clss[i] == 0:
                print('클래스 번호: ', clss[i], '\n클래스: norm', '\nnorm일 확률: ', score[i])

                # 22.11.05  옮김 ㅎㅅㅇ
                class_num = str(clss[i])
                class_name = '일반'

                cv2.rectangle(bbox, (0,0), (x, y), (0,255,0), 3)
                cv2.imwrite('./image/outro_image/norm.jpg', bbox) #저장
                norm = Image.open('./image/outro_image/norm.jpg')

                if bbox is None:
                    print('Image load failed!')
                    bbox.exit()
                
                norm_norm = Image.open("./image/outro_image/norm.jpg")
                plt.imshow(norm_norm)
                result = reader.readtext(norm_norm)
                
                if len(result[0][1]) <= 2:
                    src_hsv = cv2.cvtColor(bbox, cv2.COLOR_BGR2HSV)
                    dst1 = cv2.inRange(bbox, (0, 0, 0), (80, 80, 80))
                    cv2.imwrite('./image/outro_image/norm_black.jpg', dst1)
                    norm_black = Image.open('./image/outro_image/norm_black.jpg')
                    result = reader.readtext(norm_black)
                
                    if len(result) ==0:
                        norm_re = norm.resize((int(norm.width*2), int(norm.height)))         
                        norm_re.save('./image/outro_image/norm_re.jpg')
                        norm_re = cv2.imread('./image/outro_image/norm_re.jpg')
                        src_hsv = cv2.cvtColor(norm_re, cv2.COLOR_BGR2HSV)
                        dst1 = cv2.inRange(norm_re, (0, 0, 0), (120, 120, 120))
                        cv2.imwrite('./image/outro_image/norm_re_black.jpg', dst1)
                        norm_re_black = Image.open('./image/outro_image/norm_re_black.jpg')
                        result = reader.readtext(norm_re_black)
                        plt.imshow(norm_re_black)
                    
            elif clss[i] == 1:
                print('클래스 번호: ', clss[i], '\n클래스: elec', '\nelec일 확률: ', score[i])

                class_num = str(clss[i])
                class_name = '전기'

                cv2.rectangle(bbox, (0,0), (x, y), (0,255,0), 3)
                cv2.imwrite('./image/outro_image/elec.jpg', bbox)

                elec = Image.open('./image/outro_image/elec.jpg')

                if bbox is None:
                    print('Image load failed!')
                    bbox.exit()
                    
                elec_norm = Image.open("./image/outro_image/elec.jpg")
                plt.imshow(elec_norm)
                result = reader.readtext(elec_norm)
                
                if len(result[0][1]) <= 2:
                    src_hsv = cv2.cvtColor(bbox, cv2.COLOR_BGR2HSV)
                    dst1 = cv2.inRange(bbox, (0, 0, 0), (70, 80, 80))
                    cv2.imwrite('./image/outro_image/elec_black.jpg', dst1)
                    elec_black = Image.open('./image/outro_image/elec_black.jpg')
                    result = reader.readtext(elec_black)
                    
                    if len(result) ==0:
                        elec_re = elec.resize((int(elec.width*2), int(elec.height)))         
                        elec_re.save('./image/outro_image/elec_re.jpg')
                        elec_re = cv2.imread('./image/outro_image/elec_re.jpg')
                        src_hsv = cv2.cvtColor(elec_re, cv2.COLOR_BGR2HSV)
                        dst1 = cv2.inRange(elec_re, (0, 0, 0), (130, 130, 130))
                        cv2.imwrite('./image/outro_image/elec_re_black.jpg', dst1)
                        elec_re_black = Image.open('./image/outro_image/elec_re_black.jpg')
                        result = reader.readtext(elec_re_black)
                        plt.imshow(elec_re_black)

        # -- 이미지 에러가 나면 출력할 구문
    except :
        print('====IMG_ERR====')
    
    first_num, second_num = result_plate(result)

    out_time = car_time()

    conn = pymysql.connect(host='127.0.0.1', user='root', password='multi123', db = 'car_manage', charset='utf8')
        
    # Dict형식의 CURSOR  
    cur = conn.cursor(pymysql.cursors.DictCursor)

    # SQL 작성 
    sql_in = "select * from total_info"

    # SQL 실행
    cur.execute(query=sql_in)

    # 결과 가져오기
    result_in = cur.fetchall()

    df_in = pd.DataFrame(result_in)

    find = df_in[df_in['last_number'] == second_num]

    price = calc_price(find)

    # 출차시 total_info에서 삭제 하는 구문 추가 ==> 같은 차량이 다시 주차하는 상황을 처리하기 위함.
    cur.execute(f'delete from total_info where last_number = \"{second_num}\" ;')
    conn.commit()

    first_num = '{0:0>4}'.format(first_num)

    cur = conn.cursor()

    #아랫줄은 제일 처음에 table 생성 시에만 사용함
    #cur.execute("create table extro_info (extro_str varchar(30))")

    cur.execute(f"insert into extro_info values(\"1{class_num_y}{class_num}{second_num}{first_num}{out_time}{price}\")")

    conn.commit()
    conn.close()

if __name__ == '__main__':
    while(True):
        extro_main()