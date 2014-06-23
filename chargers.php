<?php defined('BASEPATH') OR exit('No direct script access allowed');

require_once(APPPATH.'core/api.php');

class Chargers extends Api
{    

    function __construct()
    {
        // Call the Model constructor
        parent::__construct();
		$this->load->model('charger_model');
        $this->load->model('customer_model');
        $this->load->model('api_model');
        
        // Define access rights
        $this->methods = array(
			'count_get' => array('level' => 10),
            'get_customer_get' => array('level' => 10),
        );
    }

	function count_get(){
		$q = $this->db->select('COUNT(*) AS count')->from('Charger');
		$response = $this->api_model->load($q,$this->get(),'Charger');
		$this->response($response,200);
	}

    function get_customer_get(){
		if( $this->get('UID') )
		{
			$params = array(
	            'UID'   => $this->get('UID')
	        );
	        $response = $this->charger_model->getcustomer($params);
	        $this->response($response,200);
		}
		else
		{
			$this->response(array("error" => "Bad request. UID missing."), 400);
		}
	}

	 /**
     * Create charger entry
     */
    function create_post() {

		if( $this->post('UID') && $this->post('CustomerID') && $this->post('State'))
		{
	    	$charger = new Charger_model();
	        $charger->UID = $this->post('UID');
			$charger->CustomerID = $this->post('CustomerID');
			$charger->State = $this->post('State');
			$charger->Created = date('Y-m-d H:i:s');

 
	        $response = array('results' => $charger->create());
        
	        $this->response($response, 201);
		}
		else if($this->post('State') != 'out' && $this->post('State') != 'in'){
           $this->response(array('error'=>'State \''.$this->post('State').'\' is invalid'), 400);
        }
		else
		{
			$this->response(array("error" => "Bad request. UID or CustomerID missing"), 400);
		}
    }
}
