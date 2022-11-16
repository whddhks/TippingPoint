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
    plate = ['가', '나', '다', '라', '마', '거', '너', '더', '러', '머', '버', '서', '어', '저', '고', '노', '도', '로', '모', '보', '소', '오', '조', '구', '누', '두', '루', '무', '부', '수', '우', '주', '허', '하', '호', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9']
    result_list = []
    first_num = ''
    second_num = ''
    str_cnt = 0 # 앞 번호판의 문자 (1글자)만 읽기 위함.
    for i in result :
        for j in i[1] :
            a = re.sub(' ','', i[1])
            for str in plate :
                if j == str :
                    result_list.append(j)
                    if re.search(r'[0-9]{2,3}[가-힣]{1}',a) or re.search(r'[0-9]{4}', a) :
                        if re.search(r'[0-9]{2,3}[가-힣]{1}',a) and int(re.search(r'[0-9]{2,3}',a).group()) <= 699:
                            first_num = re.search(r'[0-9]{2,3}[가-힣]{1}',a).group().strip('[-=+,#/\?:^$.@*\"※~&%ㆍ!』\\‘|\(\)\[\]\<\>`\'…》];')
                            
                        if re.search(r'[0-9]{4}', re.sub('[-=+,#/\?:^$.@*\"※~&%ㆍ!』\\‘|\(\)\[\]\<\>`\'…》];','', a)):
                            second_num = re.search(r'[0-9]{4}',a).group().strip('[-=+,#/\?:^$.@*\"※~&%ㆍ!』\\‘|\(\)\[\]\<\>`\'…》];')

                    elif second_num=='' and first_num=='' :
                        if '가'<=j<='힣' and str_cnt == 0 : 
                            str_cnt += 1
                            first_num=result_list[:result_list.index(j)+1]
                            first_num=''.join(first_num)
                            second_num=result_list[result_list.index(j)+1:]
                            second_num=''.join(first_num)

    if first_num == '' :
        first_num = 'ERRR'
    if second_num == '':
        second_num = 'ERRR'
        
        
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