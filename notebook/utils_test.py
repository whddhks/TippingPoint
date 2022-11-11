import cv2
from PIL import Image
import matplotlib.pyplot as plt
import re
import easyocr
from datetime import datetime

def webcam(cam_num, img_folder):
    # 노트북 자체의 캠은 0번
    webcam = cv2.VideoCapture(cam_num)

    #-- 웹캠 오류 처리 
    if not webcam.isOpened():
        print("WebCam is not running")
        exit()
        
    time_num= 0    
    image_num = 0
        
    #-- 웹캠 열기
    while webcam.isOpened():
            
        status, frame = webcam.read()
        time_num = time_num + 1
        
        if not status:
            break
          
        #-- 캡쳐 프레임 간격 설정
        if time_num == 1:
            image_num = image_num + 1
            cv2.imwrite('./image/'+img_folder+'/img.png', frame) #-- 본인 편의에 맞게 경로 설정 및 이미지 이름 변경
            time_num = 0
            if image_num == 1:
                break
        
    webcam.release()
                
def result_plate(result):
    result_list = []

    # 번호판으로 올 수 있는 문자만 추출 하기 위함.
    plate = ['가', '나', '다', '라', '마', '거', '너', '더', '러', '머', '버', '서', '어', '저', '고', '노', '도', '로', '모', '보', '소', '오', '조', '구', '누', '두', '루', '무', '부', '수', '우', '주', '허', '하', '호', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9']
    first_num = ''
    second_num = ''
    str_cnt = 0 # 앞 번호판의 문자 (1글자)만 읽기 위함.


    for j in result :
        for str in plate :
            if j == str :
                result_list.append(j)

        # 문자를 기준으로 앞번호 , 뒷 번호를 슬라이싱.
        if '가'<=j<='힣' and str_cnt == 0 : 
            str_cnt += 1
            first_num=result_list[:result_list.index(j)+1]
            second_num=result_list[result_list.index(j)+1:] 

            # 형식에 맞게 들어왔는지 체크.
            if 3<=len(first_num)<=4 :
                first_num = ''.join(first_num)
            else :
                first_num = 'error'

        # 앞 번호 문자 추출이후 뒷 번호를 한번 더 체크
        # 번호판 앞 번호와 뒷 번호가 분리되지 않고 result로 들어온 경우. ex) '101하 4609'
        # 번호판 앞 번호와 뒷 번호가 분리되어 result로 들어온 경우. ex)'101하' , '4609'
        # 번호판 뒷 번호에 필요없는 문자가 섞여있는 경우 ex)'107rㄱㄱ1 6540가00' 모두 걸러냄.

        else :
            result_list2 = [n for n in result_list if '0'<=n<='9' not  in result_list]
            if len(result_list2)== 0 :
                result_list2 = [n for n in result_list if '0'<=n<='9' in result_list]
            second_num = ''.join(result_list2[-4:])

    return first_num,second_num

def car_time():
    now = datetime.now()
    return '{0:0>2}'.format(str(now.hour)) + ":" + '{0:0>2}'.format(str(now.minute))


def calc_price(find):
    out = datetime.now()
    
    # 출차 에러 처리 (정산 요금이 없거나( out.hour, out.minute 실행시 0 일 때)
    #                 테이블에 입차한 차량(second_num)의 정보가 없을때 발생)
    try :
        if out.minute > int(find['intro_time'].values[0].split(':')[1]):
            h = (out.hour - int(find['intro_time'].values[0].split(':')[0]))
            m = out.minute - int(find['intro_time'].values[0].split(':')[1])
        elif out.minute < int(find['intro_time'].values[0].split(':')[1]):
            h = (out.hour - int(find['intro_time'].values[0].split(':')[0])) - 1
            m = out.minute + (60 - int(find['intro_time'].values[0].split(':')[1]))

        if find['car_elec'].values[0] == '0':
            if find['person_type'].values[0] == '0':
                price = h*60*100 + m*100
            elif find['person_type'].values[0] == '1':
                price = 0
        elif find['car_elec'].values[0] == '1':
            if find['person_type'].values[0] == '0':
                price = h*60*50 + m*50
            elif find['person_type'].values[0] == '1':
                price = 0

    # 예외 발생시 프린트 구문
    except :
        print('정산할 요금이 없습니다.')
        price = 0

    # 정산 금액이 만원 이하일때 금액 앞 문자열을 '0' 으로 채움. => str형식
    return '{0:0>5}'.format(price)

def result_arrange(result):
    first_num = 'err'
    second_num = 'err'
    first_sec_num = ''

    for a in result:
        if re.match(r'[0-9]{2,3}[가-힣]{1}[0-9]{4}',a[1].replace(' ','')):
            fir_sec_num = a[1].replace(' ','')
        if re.match(r'[0-9]{2,3}[가-힣]', re.sub('[-=+,#/\?:^$.@*\"※~&%ㆍ!』\\‘|\(\)\[\]\<\>`\'…》;]','', a[1])):
            first_num = re.sub('[-=+,#/\?:^$.@*\"※~&%ㆍ!』\\‘|\(\)\[\]\<\>`\'…》];','', a[1])
        elif re.match(r'[0-9]{4}', re.sub('[-=+,#/\?:^$.@*\"※~&%ㆍ!』\\‘|\(\)\[\]\<\>`\'…》];','', a[1])):
            second_num = re.sub('[-=+,#/\?:^$.@*\"※~&%ㆍ!』\\‘|\(\)\[\]\<\>`\'…》];','', a[1].replace(" ", ""))
            
    if first_num:
        result_str = first_num+second_num
    else:
        result_str = fir_sec_num
        
    return result_str