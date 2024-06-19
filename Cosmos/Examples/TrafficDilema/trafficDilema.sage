statelist=[
{
	'id': 0,
	'transition':[
		{'zone': [0 , 0 , 2 , 1], 'action':"b", 'miniedge': [{ 'target' :1, 'reset' : [0], 'prob':1.0}]}, 
		{'zone': [0 , 0 , 2 , 1], 'action':"c", 'miniedge': [{ 'target' :2, 'reset' : [0], 'prob':0.09090909090909091}, { 'target' :3, 'reset' : [0], 'prob':0.9090909090909091}]}, 
		{'zone': [0 , 0 , 2 , 1], 'action':"t", 'miniedge': [{ 'target' :4, 'reset' : [0], 'prob':1.0}]}],
	'name': "((s=0),{T=0})", 
	'redcoord': [[0, 1]]
},
{
	'id': 1,
	'transition':[
		{'zone': [42 , 1 , 45 , 1], 'action':"a", 'miniedge': [{ 'target' :5, 'reset' : [0], 'prob':1.0}]}],
	'name': "((s=1),{T=0})", 
	'redcoord': [[0, 1]]
},
{
	'id': 2,
	'transition':[
		{'zone': [0 , 0 , 20 , 1], 'action':"a", 'miniedge': [{ 'target' :5, 'reset' : [0], 'prob':1.0}]}],
	'name': "((s=3),{T=0})", 
	'redcoord': [[0, 1]]
},
{
	'id': 3,
	'transition':[
		{'zone': [0 , 0 , 140 , 1], 'action':"a", 'miniedge': [{ 'target' :5, 'reset' : [0], 'prob':1.0}]}],
	'name': "((s=4),{T=0})", 
	'redcoord': [[0, 1]]
},
{
	'id': 4,
	'transition':[
		{'zone': [3 , 1 , 6 , 1], 'action':"t", 'miniedge': [{ 'target' :6, 'reset' : [0], 'prob':0.9090909090909091}, { 'target' :7, 'reset' : [0], 'prob':0.09090909090909091}]}],
	'name': "((s=5),{T=0})", 
	'redcoord': [[0, 1]]
},
{
	'id': 5,
	'transition':[
		{'zone': [0 , 0 , 1 , 1], 'action':"l", 'miniedge': [{ 'target' :5, 'reset' : [0], 'prob':1.0}]}],
	'name': "((s=2),{T=0})", 
	'redcoord': [[0, 1]]
},
{
	'id': 6,
	'transition':[
		{'zone': [0 , 0 , 35 , 1], 'action':"a", 'miniedge': [{ 'target' :5, 'reset' : [0], 'prob':1.0}]}],
	'name': "((s=6),{T=0})", 
	'redcoord': [[0, 1]]
},
{
	'id': 7,
	'transition':[
		{'zone': [0 , 0 , 2 , 1], 'action':"t", 'miniedge': [{ 'target' :4, 'reset' : [0], 'prob':1.0}]}, 
		{'zone': [0 , 0 , 2 , 1], 'action':"a", 'miniedge': [{ 'target' :0, 'reset' : [0], 'prob':1.0}]}],
	'name': "((s=7),{T=0})", 
	'redcoord': [[0, 1]]
}];
alphabet=["b","c","t","a","l"];
cardclocks=1;
